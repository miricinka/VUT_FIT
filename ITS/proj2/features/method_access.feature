Feature: Methods with different access

		Scenario: Producent can see private method
			Given producent is loggend in
			And the method is in state private
			When Producent is on page with all methods
			Then he can see this method on page with all methods

		Scenario: Consument cannot see private method
			Given producent logs out
			When consument is on page with all methods
			Then he cannot see the private method


		Scenario: Producent can see published method
			Given producent is loggend in
			And producent publishes the method
			When Producent is on page with all methods
			Then he can see this method on page with all methods

		Scenario: Consument can see published method
			Given producent logs out
			When consument is on page with all methods
			Then he can see this method on page with all methods