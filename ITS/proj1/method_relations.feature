Feature: Creating /editing/ deleting Method relations

	Background:
	    Given Producent created a method

	Scenario: Add tool relation
		Given the method is in relation with 1 tool
		When Producent edits the method
		And adds relation to 1 other tool
		Then on the page of the method can be seen relation with 2 tools

	Scenario: Remove tool relation
		Given the method is in relation with 2 tools
		When Producent edits the method
		And removes all relations with tools
		Then on the page of the method can be seen relation with 0 tools

	Scenario: Add test case relation
		Given the method is not in relation with any test case
		When Producent edits the method
		And adds relation to 1 test case
		Then on the page of the method can be seen relation with 1 test case

	Scenario: Edit test case relation
		Given the method is in relation with 1 test case
		When Producent edits the method
		And removes the relation to the test case
		And adds relation to 1 another test case
		Then on the page of the method can be seen relation with different test case


