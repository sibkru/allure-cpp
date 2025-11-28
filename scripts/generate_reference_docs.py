#!/usr/bin/env python3
"""
MrDocs XML to Starlight Markdown Converter

This script parses XML output from MrDocs and generates professional
Starlight-compatible markdown documentation for C++ API reference.

Usage:
    python3 scripts/mrdocs_to_starlight.py <xml_input_dir> <markdown_output_dir>

Example:
    python3 scripts/mrdocs_to_starlight.py docs/generated docs/src/content/docs/reference/api
"""

import sys
import os
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Dict, List, Optional, Tuple
from dataclasses import dataclass, field
import re


@dataclass
class Parameter:
    """Represents a function/method parameter."""
    name: str
    type: str
    description: str = ""


@dataclass
class Function:
    """Represents a function or method."""
    name: str
    brief: str = ""
    description: str = ""
    return_type: str = "void"
    return_description: str = ""
    parameters: List[Parameter] = field(default_factory=list)
    is_static: bool = False
    is_const: bool = False
    is_noexcept: bool = False
    template_params: List[str] = field(default_factory=list)
    namespace: str = ""
    access: str = "public"  # public, private, protected


@dataclass
class Member:
    """Represents a class member variable."""
    name: str
    type: str
    brief: str = ""
    description: str = ""
    access: str = "public"


@dataclass
class Class:
    """Represents a C++ class or struct."""
    name: str
    brief: str = ""
    description: str = ""
    namespace: str = ""
    methods: List[Function] = field(default_factory=list)
    members: List[Member] = field(default_factory=list)
    base_classes: List[str] = field(default_factory=list)
    is_template: bool = False
    template_params: List[str] = field(default_factory=list)


@dataclass
class Namespace:
    """Represents a C++ namespace."""
    name: str
    brief: str = ""
    description: str = ""
    classes: List[Class] = field(default_factory=list)
    functions: List[Function] = field(default_factory=list)


class MrDocsXMLParser:
    """Parser for MrDocs XML output."""

    def __init__(self, xml_dir: Path):
        self.xml_dir = xml_dir
        self.namespaces: Dict[str, Namespace] = {}

    def parse(self) -> Dict[str, Namespace]:
        """Parse all XML files in the input directory."""
        xml_files = list(self.xml_dir.glob("*.xml"))

        if not xml_files:
            print(f"Warning: No XML files found in {self.xml_dir}")
            return {}

        for xml_file in xml_files:
            print(f"Parsing {xml_file.name}...")
            try:
                self._parse_file(xml_file)
            except Exception as e:
                print(f"Error parsing {xml_file}: {e}")

        return self.namespaces

    def _parse_file(self, xml_file: Path):
        """Parse a single XML file."""
        tree = ET.parse(xml_file)
        root = tree.getroot()

        # Parse namespaces
        for ns_elem in root.findall(".//namespace"):
            namespace = self._parse_namespace(ns_elem)
            if namespace:
                self.namespaces[namespace.name] = namespace

        # Parse classes (may be at root or in namespace)
        for class_elem in root.findall(".//class"):
            cls = self._parse_class(class_elem)
            if cls:
                ns_name = cls.namespace or "global"
                if ns_name not in self.namespaces:
                    self.namespaces[ns_name] = Namespace(name=ns_name)
                self.namespaces[ns_name].classes.append(cls)

        # Parse functions
        for func_elem in root.findall(".//function"):
            func = self._parse_function(func_elem)
            if func:
                ns_name = func.namespace or "global"
                if ns_name not in self.namespaces:
                    self.namespaces[ns_name] = Namespace(name=ns_name)
                self.namespaces[ns_name].functions.append(func)

    def _parse_namespace(self, elem: ET.Element) -> Optional[Namespace]:
        """Parse a namespace element."""
        name = elem.get("name") or elem.findtext("name")
        if not name:
            return None

        namespace = Namespace(
            name=name,
            brief=self._get_doc_text(elem, "brief"),
            description=self._get_doc_text(elem, "description")
        )

        # Parse nested classes
        for class_elem in elem.findall("class"):
            cls = self._parse_class(class_elem)
            if cls:
                cls.namespace = name
                namespace.classes.append(cls)

        # Parse namespace-level functions only (direct children, not nested in classes)
        for func_elem in elem.findall("function"):
            func = self._parse_function(func_elem)
            if func:
                func.namespace = name
                namespace.functions.append(func)

        return namespace

    def _parse_class(self, elem: ET.Element) -> Optional[Class]:
        """Parse a class element."""
        name = elem.get("name") or elem.findtext("name")
        if not name:
            return None

        cls = Class(
            name=name,
            brief=self._get_doc_text(elem, "brief"),
            description=self._get_doc_text(elem, "description")
        )

        # Parse template parameters
        for template_elem in elem.findall(".//template-param"):
            param_name = template_elem.get("name") or template_elem.findtext("name")
            if param_name:
                cls.template_params.append(param_name)
                cls.is_template = True

        # Parse base classes
        for base_elem in elem.findall(".//base"):
            base_name = base_elem.get("name") or base_elem.text
            if base_name:
                cls.base_classes.append(base_name)

        # Parse methods (in MrDocs, methods are <function> elements inside <class>)
        for method_elem in elem.findall("function"):
            method = self._parse_function(method_elem, is_method=True)
            if method:
                cls.methods.append(method)

        # Parse member variables
        for member_elem in elem.findall(".//member"):
            member = self._parse_member(member_elem)
            if member:
                cls.members.append(member)

        return cls

    def _parse_function(self, elem: ET.Element, is_method: bool = False) -> Optional[Function]:
        """Parse a function or method element."""
        name = elem.get("name") or elem.findtext("name")
        if not name:
            return None

        # Get return type from <return><type> structure
        return_type = "void"
        return_elem = elem.find("return/type")
        if return_elem is not None:
            return_type = return_elem.get("name", "void")

        func = Function(
            name=name,
            brief=self._get_doc_text(elem, "brief"),
            description=self._get_doc_text(elem, "description"),
            return_type=return_type,
            return_description=self._get_doc_text(elem, "returns"),
            access=elem.get("access", "public")
        )

        # Parse attributes
        func.is_static = elem.get("static", "false").lower() == "true"
        func.is_const = elem.get("const", "false").lower() == "true"
        func.is_noexcept = elem.get("noexcept", "false").lower() == "true"

        # Parse template parameters
        for template_elem in elem.findall(".//template-param"):
            param_name = template_elem.get("name") or template_elem.findtext("name")
            if param_name:
                func.template_params.append(param_name)

        # Parse parameters
        # First get parameter names and types from <param> elements
        param_list = []
        for param_elem in elem.findall("param"):
            param_name = param_elem.get("name", "")
            # Get type - it might be nested
            type_elem = param_elem.find("type")
            param_type = ""
            if type_elem is not None:
                param_type = type_elem.get("name", "")
                if not param_type:
                    # Handle pointer types
                    if type_elem.get("class") == "pointer":
                        pointee = type_elem.find("pointee-type")
                        if pointee is not None:
                            cv = pointee.get("cv-qualifiers", "")
                            base_type = pointee.get("name", "void")
                            param_type = f"{cv} {base_type}*".strip()

            param_list.append({"name": param_name, "type": param_type})

        # Now get parameter descriptions from doc/param elements
        doc_elem = elem.find("doc")
        if doc_elem is not None:
            for param_data in param_list:
                param_name = param_data["name"]
                description = ""

                # Find matching param in doc
                for doc_param in doc_elem.findall(f"param[@name='{param_name}']"):
                    text_elem = doc_param.find("text")
                    if text_elem is not None and text_elem.text:
                        description = text_elem.text.strip()
                        break

                param = Parameter(
                    name=param_name,
                    type=param_data["type"],
                    description=description
                )
                func.parameters.append(param)
        else:
            # No doc, just add params without descriptions
            for param_data in param_list:
                param = Parameter(
                    name=param_data["name"],
                    type=param_data["type"],
                    description=""
                )
                func.parameters.append(param)

        return func

    def _parse_member(self, elem: ET.Element) -> Optional[Member]:
        """Parse a member variable element."""
        name = elem.get("name") or elem.findtext("name")
        if not name:
            return None

        return Member(
            name=name,
            type=self._get_text(elem, "type", ""),
            brief=self._get_doc_text(elem, "brief"),
            description=self._get_doc_text(elem, "description"),
            access=elem.get("access", "public")
        )

    def _get_doc_text(self, elem: ET.Element, tag: str, default: str = "") -> str:
        """Get documentation text from MrDocs XML structure.
        MrDocs structure: <doc><brief><text>content</text></brief></doc>
        """
        doc_elem = elem.find('doc')
        if doc_elem is None:
            return default

        target = doc_elem.find(tag)
        if target is None:
            return default

        # Get all text elements and join them
        texts = []
        for text_elem in target.findall('.//text'):
            if text_elem.text:
                texts.append(text_elem.text.strip())

        return ' '.join(texts) if texts else default

    def _get_text(self, elem: ET.Element, tag: str, default: str = "") -> str:
        """Safely get text from an XML element (for non-doc elements)."""
        child = elem.find(tag)
        if child is not None and child.text:
            return child.text.strip()
        return default


class StarlightMarkdownGenerator:
    """Generates Starlight-compatible markdown from parsed C++ structures."""

    def __init__(self, output_dir: Path):
        self.output_dir = output_dir
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def generate(self, namespaces: Dict[str, Namespace]):
        """Generate markdown files for all namespaces."""
        # Only generate docs for the allure namespace, skip others
        if "allure" in namespaces:
            namespace = namespaces["allure"]
            # Generate class documentation directly in output_dir
            for cls in namespace.classes:
                self._generate_class_docs(cls, self.output_dir)

            # Generate documentation for namespace-level functions (like step())
            if namespace.functions:
                self._generate_functions_page(namespace.functions, self.output_dir)

    def _generate_namespace_docs(self, namespace: Namespace):
        """Generate documentation for a namespace."""
        # Create a directory for the namespace
        ns_dir = self.output_dir / namespace.name.replace("::", "/")
        ns_dir.mkdir(parents=True, exist_ok=True)

        # Generate index page for namespace
        if namespace.brief or namespace.description or namespace.classes or namespace.functions:
            self._generate_namespace_index(namespace, ns_dir)

        # Generate class documentation
        for cls in namespace.classes:
            self._generate_class_docs(cls, ns_dir)

    def _generate_namespace_index(self, namespace: Namespace, output_dir: Path):
        """Generate index page for a namespace."""
        output_file = output_dir / "index.md"

        with open(output_file, "w", encoding="utf-8") as f:
            # Frontmatter
            f.write("---\n")
            f.write(f"title: {namespace.name} namespace\n")
            f.write(f"description: API reference for the {namespace.name} namespace\n")
            f.write("---\n\n")

            # Brief and description
            if namespace.brief:
                f.write(f"{namespace.brief}\n\n")

            if namespace.description:
                f.write(f"{namespace.description}\n\n")

            # Classes
            if namespace.classes:
                f.write("## Classes\n\n")
                for cls in namespace.classes:
                    class_link = f"{cls.name}.md"
                    brief = cls.brief or "No description available"
                    f.write(f"- [`{cls.name}`]({class_link}) - {brief}\n")
                f.write("\n")

            # Functions
            if namespace.functions:
                f.write("## Functions\n\n")
                for func in namespace.functions:
                    self._write_function(f, func)
                    f.write("\n")

        print(f"Generated {output_file}")

    def _generate_class_docs(self, cls: Class, output_dir: Path):
        """Generate documentation for a class."""
        output_file = output_dir / f"{cls.name}.md"

        with open(output_file, "w", encoding="utf-8") as f:
            # Frontmatter
            f.write("---\n")
            f.write(f"title: {cls.name}\n")
            f.write(f"description: API reference for the {cls.name} class\n")
            f.write("---\n\n")

            if cls.brief:
                f.write(f"{cls.brief}\n\n")

            # Template declaration
            if cls.is_template:
                f.write("```cpp\n")
                template_str = ", ".join(f"typename {p}" for p in cls.template_params)
                f.write(f"template <{template_str}>\n")
                f.write(f"class {cls.name}\n")
                f.write("```\n\n")

            # Description
            if cls.description:
                f.write("## Description\n\n")
                f.write(f"{cls.description}\n\n")

            # Base classes
            if cls.base_classes:
                f.write("## Inheritance\n\n")
                for base in cls.base_classes:
                    f.write(f"- Inherits from: `{base}`\n")
                f.write("\n")

            # Public methods
            public_methods = [m for m in cls.methods if m.access == "public"]
            if public_methods:
                f.write("## Public Methods\n\n")
                for method in public_methods:
                    self._write_function(f, method, is_method=True)
                    f.write("\n")

            # Public members
            public_members = [m for m in cls.members if m.access == "public"]
            if public_members:
                f.write("## Public Members\n\n")
                for member in public_members:
                    self._write_member(f, member)
                    f.write("\n")

        print(f"Generated {output_file}")

    def _generate_functions_page(self, functions: List[Function], output_dir: Path):
        """Generate a page documenting namespace-level functions."""
        output_file = output_dir / "functions.md"

        with open(output_file, "w", encoding="utf-8") as f:
            # Frontmatter
            f.write("---\n")
            f.write("title: Functions\n")
            f.write("description: Namespace-level utility functions\n")
            f.write("---\n\n")

            f.write("Utility functions available in the `allure` namespace.\n\n")

            for func in functions:
                self._write_function(f, func)
                f.write("\n")

        print(f"Generated {output_file}")

    def _generate_global_docs(self, namespace: Namespace):
        """Generate documentation for global scope items."""
        if not namespace.classes and not namespace.functions:
            return

        output_file = self.output_dir / "globals.md"

        with open(output_file, "w", encoding="utf-8") as f:
            f.write("---\n")
            f.write("title: Global Scope\n")
            f.write("description: Global functions and classes\n")
            f.write("---\n\n")

            if namespace.classes:
                f.write("## Classes\n\n")
                for cls in namespace.classes:
                    self._write_class_brief(f, cls)

            if namespace.functions:
                f.write("## Functions\n\n")
                for func in namespace.functions:
                    self._write_function(f, func)

        print(f"Generated {output_file}")

    def _write_class_brief(self, f, cls: Class):
        """Write a brief class overview."""
        f.write(f"### `{cls.name}`\n\n")
        if cls.brief:
            f.write(f"{cls.brief}\n\n")

    def _write_function(self, f, func: Function, is_method: bool = False):
        """Write function documentation in markdown."""
        # Function name as header
        f.write(f"### {func.name}\n\n")

        if func.brief:
            f.write(f"{func.brief}\n\n")

        # Function signature
        f.write("```cpp\n")

        # Template parameters
        if func.template_params:
            template_str = ", ".join(f"typename {p}" for p in func.template_params)
            f.write(f"template <{template_str}>\n")

        # Static keyword
        if func.is_static and is_method:
            f.write("static ")

        # Return type and name
        f.write(f"{func.return_type} {func.name}(")

        # Parameters
        if func.parameters:
            param_strs = []
            for param in func.parameters:
                param_str = f"{param.type} {param.name}" if param.name else param.type
                param_strs.append(param_str)
            f.write(", ".join(param_strs))

        f.write(")")

        # Const and noexcept
        if func.is_const:
            f.write(" const")
        if func.is_noexcept:
            f.write(" noexcept")

        f.write("\n```\n\n")

        # Description
        if func.description:
            f.write(f"{func.description}\n\n")

        # Parameters section
        if func.parameters:
            f.write("**Parameters:**\n\n")
            f.write("| Name | Type | Description |\n")
            f.write("|------|------|-------------|\n")
            for param in func.parameters:
                desc = param.description or "No description"
                # Escape pipe characters in descriptions
                desc = desc.replace("|", "\\|")
                f.write(f"| `{param.name}` | `{param.type}` | {desc} |\n")
            f.write("\n")

        # Return value
        if func.return_type != "void":
            f.write("**Returns:**\n\n")
            return_desc = func.return_description or f"Returns a value of type `{func.return_type}`"
            f.write(f"{return_desc}\n\n")

    def _write_member(self, f, member: Member):
        """Write member variable documentation."""
        f.write(f"### `{member.name}`\n\n")

        f.write("```cpp\n")
        f.write(f"{member.type} {member.name}\n")
        f.write("```\n\n")

        if member.brief:
            f.write(f"{member.brief}\n\n")

        if member.description:
            f.write(f"{member.description}\n\n")


def main():
    if len(sys.argv) < 3:
        print("Usage: python3 mrdocs_to_starlight.py <xml_input_dir> <markdown_output_dir>")
        print("\nExample:")
        print("  python3 scripts/mrdocs_to_starlight.py docs/generated docs/src/content/docs/reference/api")
        sys.exit(1)

    xml_input_dir = Path(sys.argv[1])
    markdown_output_dir = Path(sys.argv[2])

    if not xml_input_dir.exists():
        print(f"Error: Input directory '{xml_input_dir}' does not exist")
        sys.exit(1)

    print(f"Parsing MrDocs XML from: {xml_input_dir}")
    print(f"Generating Starlight markdown to: {markdown_output_dir}")
    print()

    # Parse XML
    parser = MrDocsXMLParser(xml_input_dir)
    namespaces = parser.parse()

    if not namespaces:
        print("Warning: No namespaces found. Check your XML input.")
        sys.exit(0)

    print(f"\nFound {len(namespaces)} namespace(s)")
    for ns_name, ns in namespaces.items():
        print(f"  - {ns_name}: {len(ns.classes)} classes, {len(ns.functions)} functions")

    # Generate markdown
    print("\nGenerating markdown files...")
    generator = StarlightMarkdownGenerator(markdown_output_dir)
    generator.generate(namespaces)

    print("\nDone!")


if __name__ == "__main__":
    main()
