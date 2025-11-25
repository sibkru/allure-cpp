if(ALLURE_ENABLE_GOOGLETEST)
	add_subdirectory(GoogleTest)
endif()

if(ALLURE_ENABLE_CPPUTEST)
	add_subdirectory(CppUTest)
endif()
