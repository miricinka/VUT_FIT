from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains

#Scenario: Entering method editing
@given(u'a producent is logged in')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.ID, "personaltools-login").click()
    context.driver.find_element(By.ID, "__ac_name").send_keys("itsadmin")
    context.driver.find_element(By.ID, "__ac_password").send_keys("itsadmin")
    context.driver.find_element(By.CSS_SELECTOR, ".pattern-modal-buttons > #buttons-login").click()


@given(u'producent is on page with existing method')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.LINK_TEXT, "Methods").click()
    context.driver.find_element(By.LINK_TEXT, "Combinatorial Testing").click()


@when(u'producent clicks on "Edit"')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, "#contentview-edit span:nth-child(2)").click()


@then(u'page with "Edit Method" is shown')
def step_impl(context):
		assert context.driver.find_element(By.CSS_SELECTOR, ".documentFirstHeading").text == "Edit Method"

#Scenario: Editing method
@given(u'producent is on page "Edit Method"')
def step_impl(context):
    assert context.driver.find_element(By.CSS_SELECTOR, ".documentFirstHeading").text == "Edit Method"


@given(u'edits information')
def step_impl(context):
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").click()
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").send_keys("")
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").send_keys(" Editing")


@when(u'producent clicks on "Save"')
def step_impl(context):
    context.driver.find_element(By.ID, "form-buttons-save").click()


@then(u'info is displayed')
def step_impl(context):
    assert context.driver.find_element_by_xpath("/html/body[@id='visual-portal-wrapper']/div[@class='outer-wrapper']/div[@class='container']/div[@class='row']/aside[@id='global_statusmessage']/div[@class='portalMessage info']/strong").text == "Info"


@then(u'method page with edited information is shown')
def step_impl(context):
		assert context.driver.find_element(By.CSS_SELECTOR, ".documentFirstHeading").text == "Combinatorial Testing Editing"

#Scenario: Creating new Method - not filling required fields
@given(u'a web browser is on "Add Method" page')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Methods").click()
    context.driver.find_element(By.CSS_SELECTOR, "#plone-contentmenu-factories .plone-toolbar-title").click()
    context.driver.find_element(By.ID, "method").click()


@given(u'required fields are not filled')
def step_impl(context):
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").click()
    context.driver.find_element(By.ID, "form-widgets-IBasic-description").click()
    context.driver.find_element(By.ID, "form-widgets-method_purpose").click()


@then(u'error "There were some errors. "is shown')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, ".portalMessage").text == "Error"
    assert context.driver.find_element_by_xpath("/html/body[@id='visual-portal-wrapper']/div[@class='outer-wrapper']/div[@class='container']/main[@id='main-container']/div[@class='col-xs-12 col-sm-12 col-md-12']/div[@class='row']/div[@class='col-xs-12 col-sm-12']/article[@id='content']/div[@id='content-core']/dl[@class='portalMessage error']/dd").text == "There were some errors."


#Scenario: Creating new Method - filling required fields
@given(u'required fields are filled')
def step_impl(context):
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").click()
    context.driver.find_element(By.ID, "form-widgets-IBasic-title").send_keys("New Method")
    context.driver.find_element(By.ID, "form-widgets-method_purpose").click()
    context.driver.find_element(By.ID, "form-widgets-method_purpose").send_keys("For cool stuff")
    context.driver.switch_to.frame(0)
    context.driver.switch_to.default_content()
    context.driver.execute_script("window.scrollTo(0,1166)")
    context.driver.switch_to.frame(0)
    element = context.driver.find_element(By.ID, "tinymce")
    context.driver.execute_script("if(arguments[0].contentEditable === 'true') {arguments[0].innerText = 'A really cool method for cool stuff'}", element)
    context.driver.switch_to.default_content()
    context.driver.switch_to.frame(1)
    context.driver.switch_to.default_content()
    context.driver.execute_script("window.scrollTo(0,1671)")
    context.driver.switch_to.frame(1)
    element = context.driver.find_element(By.ID, "tinymce")
    context.driver.execute_script("if(arguments[0].contentEditable === 'true') {arguments[0].innerText = 'the best'}", element)
    context.driver.switch_to.default_content()
    context.driver.switch_to.frame(2)
    context.driver.switch_to.default_content()
    context.driver.execute_script("window.scrollTo(0,2409)")
    context.driver.switch_to.frame(2)
    element = context.driver.find_element(By.ID, "tinymce")
    context.driver.execute_script("if(arguments[0].contentEditable === 'true') {arguments[0].innerText = 'none'}", element)
    context.driver.switch_to.default_content()


@then(u'method is created')
def step_impl(context):
    assert context.driver.find_element(By.CSS_SELECTOR, ".documentFirstHeading").text == "New Method"
    assert context.driver.find_element_by_xpath("/html/body[@id='visual-portal-wrapper']/div[@class='outer-wrapper']/div[@class='container']/main[@id='main-container']/div[@class='col-xs-12 col-sm-12 col-md-9 col-md-push-3']/div[@class='row']/div[@class='col-xs-12 col-sm-12']/article[@id='content']/header/h1[@class='documentFirstHeading']").text == "New Method"


@then(u'can be seen on page with all methods')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Methods").click()
    context.driver.find_element(By.CSS_SELECTOR, ".entry:nth-child(2) > header").text == "New Method"

#Scenario: Wanting to delete a method
@given(u'producent is on page with new method')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "New Method").click()


@when(u'producent clicks on "Actions"')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, "#plone-contentmenu-actions .plone-toolbar-title").click()


@when(u'then clicks on "Delete"')
def step_impl(context):
    context.driver.find_element(By.ID, "plone-contentmenu-actions-delete").click()


@then(u'popup is shown "Do you really want to delete this folder and all its contents?"')
def step_impl(context):
    assert context.driver.find_element(By.CSS_SELECTOR, ".plone-modal-title").text == "Do you really want to delete this folder and all its contents?"

#Scenario: Deleting method
@given(u'producent is on method page with popup "Do you really want to delete this folder and all its contents?"')
def step_impl(context):
    assert context.driver.find_element(By.CSS_SELECTOR, ".plone-modal-title").text == "Do you really want to delete this folder and all its contents?"


@when(u'producent clicks on "Delete"')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, ".pattern-modal-buttons > #form-buttons-Delete").click()


@then(u'info about deleting is shown')
def step_impl(context):
    assert context.driver.find_element_by_xpath("/html/body[@id='visual-portal-wrapper']/div[@class='outer-wrapper']/div[@class='container']/div[@class='row']/aside[@id='global_statusmessage']/div[@class='portalMessage info']/strong").text == "Info"

    #cleanup
    context.driver.find_element(By.CSS_SELECTOR, "#portal-personaltools span:nth-child(2)").click()
    context.driver.find_element(By.ID, "personaltools-logout").click()

