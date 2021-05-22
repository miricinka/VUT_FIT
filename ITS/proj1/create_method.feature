Feature: Creating /editing/ deleting Methods

	Background:
    	Given a producent is logged in

    Scenario: Choosing item from "Add new..."
	    Given web browser is on the home page
	    When producent clicks on "Add new..."
	    Then menu with options is shown:
	    	| Add new...            | 
	    	| Collection            |
	    	| Evaluation Scenario   |
	    	| Event                 |
	    	| File                  |
	    	| Folder                | 
	    	| Image                 |
	    	| Link                  |
	    	| Method                |
	    	| News Item             |
	    	| Organization          |
	    	| Requirement           |
	    	| Standart              |
	    	| Test Case             |
	    	| Tool                  |
	    	| Use Case              |

    Scenario Outline: Get to page with Add item
	    Given web browser is on the home page
	    When producent clicks on "Add new..."
	    And choses <item>
	    Then page with "Add <item>" is shown
	    	Examples:
	    		| item            | 
	    		| Folder          | 
	    		| Image           |
	    		| Method          |
	    		| Requirement     |
	    		| Standart        |
	    		| Test Case       |
	    		| Tool        	  |
	    		| Use Case        |

    Scenario: Creating new Method - not filling required fields
	    Given a web browser is on "Add Method" page
	    And required fields are not filled
	    When producent clicks on "Save"
	    Then method is not created
	    And cannot be seen on page with all methods

    Scenario: Creating new Method - filling required fields
	    Given a web browser is on "Add Method" page
	    And required fields are filled
	    When producent clicks on "Save"
	    Then method is created
	    And can be seen on page with all methods

    Scenario: Entering method editing
	    Given producent is on page with existing method
	    When producent clicks on "Edit"
	    Then page with "Edit Method" is shown

    Scenario: Editing method
	    Given producent is on page "Edit Method"
	    And edits information
	    When producent clicks on "Save"
	    Then info "Changes saved" is displayed
	    And method page with edited information is shown

    Scenario: Wanting to delete a method
	    Given producent is on page with existing method
	    When producent clicks on "Actions"
	    And then clicks on "Delete"
	    Then popup is shown "Do you really want to delete this folder and all its contents?"

    Scenario: Deleting method
	    Given producent is on method page with popup "Do you really want to delete this folder and all its contents?"
	    And buttons "Delete" and "Cancel" are shown
	    When producent clicks on "Delete"
	    Then the method is not on page with existing methods



