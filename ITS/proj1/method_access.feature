Feature: Methods with different access

	Background:
	    Given Producent created a method

	Scenario: Producent can see created but yet not published method
		Given the method is in state private
		When Producent is on page with all methods
		Then he can see this method on page with all methods
		And the method is color red

	Scenario: Consument cannot see private method
		Given the method is in state private
		When consument is on page with all methods
		Then he cannot see the method

	Scenario: Admin can see created method with pending review
		Given Producent submited method for publication
		When Producent is on page with all methods
		Then he can see this this method on page with all methods
		And the method is color green

	Scenario: Consument cannot see method sent to publication
		Given Producent submited method for publication
		When consument is on page with all methods
		Then he cannot see the method

	Scenario: Producent can see published method
		Given the method is published
		When Producent is on page with all methods
		Then he can see the method on page with all methods
		And the method is color blue

	Scenario: Consument can see published method
		Given the method is published
		When consument is on page with all methods
		Then he can see the method on page with all methods

	Scenario: Admin can see method that has been sent back
		Given the method is published
		When producents is on page with the method
		And clicks on "Send back"
		Then he can see the method on page with all methods
		And the method is in state "Private"
