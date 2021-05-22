Feature: Creating /editing/ deleting Methods

  Scenario: Entering method editing
  		Given a producent is logged in
	    And producent is on page with existing method
	    When producent clicks on "Edit"
	    Then page with "Edit Method" is shown

	Scenario: Editing method
	    Given producent is on page "Edit Method"
	    And edits information
	    When producent clicks on "Save"
	    Then info is displayed
	    And method page with edited information is shown

	Scenario: Creating new Method - not filling required fields
	    Given a web browser is on "Add Method" page
	    And required fields are not filled
	    When producent clicks on "Save"
	    Then error "There were some errors. "is shown

	Scenario: Creating new Method - filling required fields
	    Given a web browser is on "Add Method" page
	    And required fields are filled
	    When producent clicks on "Save"
	    Then method is created
	    And can be seen on page with all methods

	Scenario: Wanting to delete a method
	    Given producent is on page with new method
	    When producent clicks on "Actions"
	    And then clicks on "Delete"
	    Then popup is shown "Do you really want to delete this folder and all its contents?"

	Scenario: Deleting method
	    Given producent is on method page with popup "Do you really want to delete this folder and all its contents?"
	    When producent clicks on "Delete"
	    Then info about deleting is shown



