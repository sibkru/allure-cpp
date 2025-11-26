#pragma once

#include "Framework/FrameworkCapabilities.h"
#include "Framework/ITestStatusProvider.h"

#include <memory>


namespace allure {

	/**
	 * @brief Main interface for test framework adapters.
	 *
	 * This is the primary abstraction point for integrating different test frameworks
	 * with the Allure reporting system. Each supported framework (GoogleTest, CppUTest,
	 * Catch2, etc.) implements this interface.
	 *
	 * The adapter is responsible for:
	 * 1. Registering itself with the test framework's event/listener system
	 * 2. Translating framework-specific events into ITestLifecycleListener calls
	 * 3. Providing framework-specific status query capabilities
	 * 4. Describing the framework's capabilities
	 *
	 * Usage Pattern:
	 * @code
	 * // Create and initialize adapter
	 * auto adapter = std::make_unique<GTestAdapter>(lifecycleListener);
	 * adapter->initialize();
	 *
	 * // Query capabilities
	 * auto caps = adapter->getCapabilities();
	 * if (caps.supportsRuntimeStatus) {
	 *     auto statusProvider = adapter->createStatusProvider();
	 *     if (statusProvider->isCurrentTestFailed()) {
	 *         // Handle failure
	 *     }
	 * }
	 * @endcode
	 */
	class ITestFrameworkAdapter
	{
	public:
		virtual ~ITestFrameworkAdapter() = default;

		/**
		 * @brief Initialize the adapter and register with the test framework.
		 *
		 * This method performs framework-specific initialization:
		 * - GoogleTest: Registers an ::testing::TestEventListener
		 * - CppUTest: Installs a TestPlugin
		 * - Catch2: Registers event listeners
		 *
		 * This must be called before any tests run, typically during main()
		 * or static initialization.
		 *
		 * @throws std::runtime_error if initialization fails or framework is unavailable
		 */
		virtual void initialize() = 0;

		/**
		 * @brief Create a status provider for runtime test status queries.
		 *
		 * The status provider allows code to check if the current test has
		 * failed or been skipped during test execution.
		 *
		 * @return A new ITestStatusProvider instance, or nullptr if the framework
		 *         doesn't support runtime status queries (check getCapabilities())
		 *
		 * @note The caller owns the returned pointer. Create a new provider for
		 *       each query context if needed.
		 */
		virtual std::unique_ptr<ITestStatusProvider> createStatusProvider() const = 0;

		/**
		 * @brief Get the capabilities of the test framework.
		 *
		 * This describes what features the framework supports, allowing the
		 * reporting system to adapt its behavior accordingly.
		 *
		 * @return FrameworkCapabilities structure describing framework features
		 */
		virtual FrameworkCapabilities getCapabilities() const = 0;
	};

}
