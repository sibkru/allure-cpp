#!/bin/bash
# Script to update documentation with latest code snippets
# This script should be run from the project root directory

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Documentation Update Script ===${NC}"
echo ""

# Check we're in the right directory
if [ ! -f "CMakeLists.txt" ] || [ ! -d "test/docs-snippets" ]; then
    echo -e "${RED}Error: This script must be run from the project root directory${NC}"
    exit 1
fi

# Step 1: Build snippet tests
echo -e "${YELLOW}Step 1: Building snippet tests...${NC}"
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    cmake -B build -DALLURE_ENABLE_GOOGLETEST=ON -DALLURE_ENABLE_CPPUTEST=ON
else
    echo "Reconfiguring build with snippet tests enabled..."
    cmake -B build -DALLURE_ENABLE_GOOGLETEST=ON -DALLURE_ENABLE_CPPUTEST=ON
fi

# Discover DocsSnippet targets dynamically so new examples are picked up automatically
SNIPPET_PREFIX="DocsSnippet_"
SNIPPET_TARGETS=$(python3 scripts/list_docs_snippet_targets.py "${SNIPPET_PREFIX}")

if [ -z "${SNIPPET_TARGETS}" ]; then
    echo -e "${RED}No snippet targets found (pattern ${SNIPPET_PREFIX})${NC}"
    exit 1
fi

echo "Found snippet targets:"
echo "${SNIPPET_TARGETS}" | sed 's/^/  - /'

# Build all snippet targets
for target in ${SNIPPET_TARGETS}; do
    echo "Building ${target}..."
    cmake --build build --target "${target}" || {
        echo -e "${RED}Failed to build ${target}${NC}"
        exit 1
    }
done
echo -e "${GREEN}✓ All snippet tests built successfully${NC}"
echo ""

# Run the snippet tests to verify they work at runtime
echo -e "${YELLOW}Running snippet tests...${NC}"

for target in ${SNIPPET_TARGETS}; do
    binary="./build/bin/${target}"
    if [ ! -x "${binary}" ]; then
        echo -e "${YELLOW}Warning: binary ${binary} not found or not executable; skipping run${NC}"
        continue
    fi
    echo "Running ${target}..."
    "${binary}" || {
        echo -e "${RED}${target} failed at runtime${NC}"
        exit 1
    }
    rm -rf allure-results
done

echo -e "${GREEN}✓ All snippet tests passed${NC}"
echo ""

# Step 2: Generate API reference documentation with MrDocs
echo -e "${YELLOW}Step 2: Generating API reference documentation...${NC}"

# Check if mrdocs is available
if ! command -v mrdocs &> /dev/null; then
    echo -e "${YELLOW}Warning: mrdocs not found in PATH. Skipping API reference generation.${NC}"
    echo -e "${YELLOW}Install mrdocs from: https://mrdocs.com${NC}"
else
    # On macOS, ensure SDKROOT is set so system headers like fcntl.h/sys/stat.h are found
    if [ "$(uname)" = "Darwin" ] && [ -z "${SDKROOT}" ]; then
        export SDKROOT=$(xcrun --show-sdk-path 2>/dev/null || true)
    fi

    # Ensure compilation database exists
    if [ ! -f "build/compile_commands.json" ]; then
        echo "Generating compilation database..."
        cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DALLURE_ENABLE_GOOGLETEST=ON -DALLURE_ENABLE_CPPUTEST=ON
    fi

    # Run mrdocs to generate XML
    echo "Running mrdocs..."
    mrdocs --config=mrdocs.yml || {
        echo -e "${RED}Failed to run mrdocs${NC}"
        exit 1
    }

    # Convert XML to Starlight markdown
    echo "Converting to Starlight markdown..."
    python3 scripts/generate_reference_docs.py docs/generated docs/src/content/docs/reference || {
        echo -e "${RED}Failed to generate reference docs${NC}"
        exit 1
    }

    echo -e "${GREEN}✓ API reference documentation generated${NC}"
fi
echo ""

# Step 3: Extract snippets
echo -e "${YELLOW}Step 3: Extracting snippets from source files...${NC}"
python3 scripts/extract_snippets.py || {
    echo -e "${RED}Failed to extract snippets${NC}"
    exit 1
}
echo -e "${GREEN}✓ Snippets extracted successfully${NC}"
echo ""

# Step 4: Inject snippets into documentation
echo -e "${YELLOW}Step 4: Injecting snippets into documentation...${NC}"
python3 scripts/inject_snippets.py || {
    echo -e "${RED}Failed to inject snippets${NC}"
    exit 1
}
echo -e "${GREEN}✓ Snippets injected successfully${NC}"
echo ""

# Step 5: Generate example Allure reports (optional)
if [ "$1" = "--build-docs" ] || [ "$1" = "--with-examples" ]; then
    echo -e "${YELLOW}Step 5: Generating example Allure reports...${NC}"

    # Check if allure CLI is available
    if ! command -v allure &> /dev/null; then
        echo -e "${YELLOW}Warning: allure CLI not found. Skipping example report generation.${NC}"
        echo -e "${YELLOW}Install allure from: https://docs.qameta.io/allure/${NC}"
    else
        bash scripts/generate_trend_reports.sh || {
            echo -e "${RED}Failed to generate example reports${NC}"
            exit 1
        }

        # Reports already generated to docs/public by generate_trend_reports.sh

        echo -e "${GREEN}✓ Example Allure reports generated${NC}"
    fi
    echo ""
fi

# Step 6: Build documentation (optional)
if [ "$1" = "--build-docs" ]; then
    echo -e "${YELLOW}Step 6: Building documentation site...${NC}"
    cd docs
    if [ ! -d "node_modules" ]; then
        echo "Installing npm dependencies..."
        npm install
    fi
    npm run build || {
        echo -e "${RED}Failed to build documentation site${NC}"
        exit 1
    }
    cd ..
    echo -e "${GREEN}✓ Documentation site built successfully${NC}"
    echo ""
fi

echo -e "${GREEN}=== Documentation update complete! ===${NC}"
echo ""
echo "Next steps:"
echo "  - Review changes in docs/src/content/docs/"
if [ "$1" != "--build-docs" ]; then
    echo "  - Build the documentation site: cd docs && npm run build"
fi
echo "  - Commit the updated documentation"
