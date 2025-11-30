#!/usr/bin/env bash
set -euo pipefail

# This script automates generating two-point Allure trends (fail → pass)
# for both GoogleTest and CppUTest examples. It:
# 1) Forces calculator bugs, builds, runs all examples, and captures failing results.
# 2) Enables BUGS_FIXED, rebuilds, appends failing run to history (Allure 3), runs again, and publishes reports.
#
# Prerequisites: allure CLI on PATH, cmake, C++ toolchain, git repo root.

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
DOCS_DIR="${ROOT_DIR}/docs/public"
HEADER="${ROOT_DIR}/examples/shared/Calculator.h"
HISTORY_PATH="${ROOT_DIR}/.allure/history.jsonl"
ALLURE3_CONFIG="${ROOT_DIR}/examples/allure3config.mjs"

ALLURE_BIN="${ALLURE_BIN:-allure}"

fail() { echo "ERROR: $*" >&2; exit 1; }

need_cmd() { command -v "$1" >/dev/null 2>&1 || fail "Missing required command: $1"; }

detect_allure_major() {
    local ver
    ver="$("${ALLURE_BIN}" --version 2>/dev/null | head -n1)"
    echo "${ver}" | sed -E 's/[^0-9]*([0-9]+).*/\1/'
}

generate_report() {
    local input="$1"
    local output="$2"
    local cwd="${3:-${ROOT_DIR}}"  # default to repo root if not provided
    local major
    major="$(detect_allure_major)"

    if [[ -z "${major}" ]]; then
        fail "Could not determine Allure CLI version"
    fi

    if (( major >= 3 )); then
        (cd "${cwd}" && "${ALLURE_BIN}" generate "${input}" -o "${output}" --config "${ALLURE3_CONFIG}")
    else
        (cd "${cwd}" && "${ALLURE_BIN}" generate "${input}" --clean -o "${output}")
    fi
}

need_cmd "${ALLURE_BIN}"
need_cmd cmake

ensure_define() {
    local mode="$1" # off|on
    if [[ "${mode}" == "off" ]]; then
        perl -0pi -e 's@^\s*#define BUGS_FIXED@// #define BUGS_FIXED@m' "${HEADER}"
    else
        perl -0pi -e 's@^\s*//\s*#define BUGS_FIXED@#define BUGS_FIXED@m' "${HEADER}"
    fi
}

configure_build() {
    mkdir -p "${BUILD_DIR}"
    cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
        -DALLURE_ENABLE_GOOGLETEST=ON \
        -DALLURE_ENABLE_CPPUTEST=ON \
        -DALLURE_BUILD_EXAMPLES=ON \
        -DCMAKE_BUILD_TYPE=Release
    cmake --build "${BUILD_DIR}"
}

run_examples() {
    local label="$1" # fail|pass
    local g_out="${BUILD_DIR}/allure-results-googletest-${label}"
    local c_out="${BUILD_DIR}/allure-results-cpputest-${label}"
    local build_order
    if [[ "${label}" == "fail" ]]; then
        build_order=1
    else
        build_order=2
    fi
    rm -rf "${BUILD_DIR}/allure-results"
    rm -rf "${g_out}" "${c_out}"

    # GoogleTest
    (cd "${BUILD_DIR}" && rm -rf allure-results && ALLURE_BUILD_ORDER="${build_order}" ALLURE_BUILD_NAME="googletest-${label}" ./bin/GoogleTestAllureExample || true)
    [[ -d "${BUILD_DIR}/allure-results" ]] || fail "GoogleTest run did not produce allure-results"
    mv "${BUILD_DIR}/allure-results" "${g_out}"

    # CppUTest
    (cd "${BUILD_DIR}" && rm -rf allure-results && ALLURE_BUILD_ORDER="${build_order}" ALLURE_BUILD_NAME="cpputest-${label}" ./bin/CppUTestAllureExample || true)
    [[ -d "${BUILD_DIR}/allure-results" ]] || fail "CppUTest run did not produce allure-results"
    mv "${BUILD_DIR}/allure-results" "${c_out}"
}

append_history_allure3() {
    local dir="$1"
    local major
    major="$(detect_allure_major)"
    if (( major >= 3 )); then
        (cd "${ROOT_DIR}" && "${ALLURE_BIN}" history "${dir}" --history-path "${HISTORY_PATH}" --name "Allure Report")
    fi
}

publish_report() {
    local results_dir="$1"
    local target_dir="$2"
    local cwd="$3"
    local tmp_out
    tmp_out="$(mktemp -d "${BUILD_DIR}/report.XXXXXX")"
    generate_report "${results_dir}" "${tmp_out}" "${cwd}"
    rm -rf "${target_dir}"
    mkdir -p "${target_dir}"
    cp -r "${tmp_out}/"* "${target_dir}/"
}

main() {
    echo "== Cleaning previous history and per-run directories =="
    rm -f "${HISTORY_PATH}"
    rm -rf "${BUILD_DIR}/googletest" "${BUILD_DIR}/cpputest"

    echo "== Forcing calculator bugs (failing run) =="
    ensure_define off
    configure_build
    run_examples "fail"

# Generate temp reports from failing runs so their history is appended (separate CWDs)
echo "== Generating fail reports (appends history) =="
mkdir -p "${BUILD_DIR}/googletest" "${BUILD_DIR}/cpputest"
tmp_fail_report="$(mktemp -d "${BUILD_DIR}/report.fail.XXXXXX")"
generate_report "${BUILD_DIR}/allure-results-googletest-fail" "${tmp_fail_report}/googletest" "${BUILD_DIR}/googletest"
generate_report "${BUILD_DIR}/allure-results-cpputest-fail" "${tmp_fail_report}/cpputest" "${BUILD_DIR}/cpputest"

echo "== Enabling calculator fix (passing run) =="
ensure_define on
configure_build
run_examples "pass"

# Regenerate reports after passing run (appends pass history)
echo "== Generating pass reports (appends history) =="
tmp_pass_report="$(mktemp -d "${BUILD_DIR}/report.pass.XXXXXX")"
generate_report "${BUILD_DIR}/allure-results-googletest-pass" "${tmp_pass_report}/googletest" "${BUILD_DIR}/googletest"
generate_report "${BUILD_DIR}/allure-results-cpputest-pass" "${tmp_pass_report}/cpputest" "${BUILD_DIR}/cpputest"

    echo "== Publishing reports =="
    rm -rf "${DOCS_DIR}/googletest-example" "${DOCS_DIR}/cpputest-example"
    mkdir -p "${DOCS_DIR}/googletest-example" "${DOCS_DIR}/cpputest-example"
    cp -r "${tmp_pass_report}/googletest/"* "${DOCS_DIR}/googletest-example/"
    cp -r "${tmp_pass_report}/cpputest/"* "${DOCS_DIR}/cpputest-example/"

    echo "Done. Reports with two-point trend written to:"
    echo "  - ${DOCS_DIR}/googletest-example"
    echo "  - ${DOCS_DIR}/cpputest-example"
}

main "$@"
