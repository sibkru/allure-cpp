#!/usr/bin/env bash
set -euo pipefail

# This script automates generating two-point Allure trends (fail → pass)
# for both GoogleTest and CppUTest examples. It:
# 1) Forces calculator bugs, builds, runs all examples, and captures failing results.
# 2) Enables BUGS_FIXED, rebuilds, seeds history from the failing run, runs again, and publishes reports.
#
# Prerequisites: allure CLI on PATH, cmake, C++ toolchain, git repo root.

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
DOCS_DIR="${ROOT_DIR}/docs"
HEADER="${ROOT_DIR}/examples/shared/Calculator.h"

ALLURE_BIN="${ALLURE_BIN:-allure}"

fail() { echo "ERROR: $*" >&2; exit 1; }

need_cmd() { command -v "$1" >/dev/null 2>&1 || fail "Missing required command: $1"; }

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
    rm -rf "${BUILD_DIR}/allure-results"
    rm -rf "${g_out}" "${c_out}"

    # GoogleTest
    (cd "${BUILD_DIR}" && rm -rf allure-results && ./bin/GoogleTestAllureExample || true)
    [[ -d "${BUILD_DIR}/allure-results" ]] || fail "GoogleTest run did not produce allure-results"
    mv "${BUILD_DIR}/allure-results" "${g_out}"

    # CppUTest
    (cd "${BUILD_DIR}" && rm -rf allure-results && ./bin/CppUTestAllureExample || true)
    [[ -d "${BUILD_DIR}/allure-results" ]] || fail "CppUTest run did not produce allure-results"
    mv "${BUILD_DIR}/allure-results" "${c_out}"
}

seed_and_run_pass() {
    local fail_dir="$1"
    local bin="$2"
    local out_dir="$3"

    rm -rf "${BUILD_DIR}/allure-results" "${out_dir}"
    mkdir -p "${BUILD_DIR}/allure-results"

    # Seed history from the failing run's report so we get exactly one prior point.
    local tmp_report
    tmp_report="$(mktemp -d "${BUILD_DIR}/seed.XXXXXX")"
    "${ALLURE_BIN}" generate "${fail_dir}" --clean -o "${tmp_report}"
    cp -r "${tmp_report}/history" "${BUILD_DIR}/allure-results/"

    (cd "${BUILD_DIR}" && ./bin/"${bin}" || true)
    mv "${BUILD_DIR}/allure-results" "${out_dir}"
}

publish_report() {
    local results_dir="$1"
    local target_dir="$2"
    local tmp_out
    tmp_out="$(mktemp -d "${BUILD_DIR}/report.XXXXXX")"
    "${ALLURE_BIN}" generate "${results_dir}" --clean -o "${tmp_out}"
    rm -rf "${target_dir}"
    mkdir -p "${target_dir}"
    cp -r "${tmp_out}/"* "${target_dir}/"
}

main() {
    echo "== Forcing calculator bugs (failing run) =="
    ensure_define off
    configure_build
    run_examples "fail"

    echo "== Enabling calculator fix (passing run) =="
    ensure_define on
    configure_build

    seed_and_run_pass "${BUILD_DIR}/allure-results-googletest-fail" "GoogleTestAllureExample" "${BUILD_DIR}/allure-results-googletest-pass"
    seed_and_run_pass "${BUILD_DIR}/allure-results-cpputest-fail" "CppUTestAllureExample" "${BUILD_DIR}/allure-results-cpputest-pass"

    echo "== Publishing reports =="
    publish_report "${BUILD_DIR}/allure-results-googletest-pass" "${DOCS_DIR}/googletest-example"
    publish_report "${BUILD_DIR}/allure-results-cpputest-pass" "${DOCS_DIR}/cpputest-example"

    echo "Done. Reports with two-point trend written to:"
    echo "  - ${DOCS_DIR}/googletest-example"
    echo "  - ${DOCS_DIR}/cpputest-example"
}

main "$@"
