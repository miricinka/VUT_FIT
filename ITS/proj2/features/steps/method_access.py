from behave import *
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains


#Scenario: Producent can see created but yet not published method
@given(u'producent is loggend in')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.ID, "personaltools-login").click()
    context.driver.find_element(By.ID, "__ac_name").send_keys("itsadmin")
    context.driver.find_element(By.ID, "__ac_password").send_keys("itsadmin")
    context.driver.find_element(By.CSS_SELECTOR, ".pattern-modal-buttons > #buttons-login").click()

@given(u'the method is in state private')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.LINK_TEXT, "Methods").click()
    context.driver.find_element(By.LINK_TEXT, "Combinatorial Testing Editing").click()
    context.driver.find_element(By.CSS_SELECTOR, ".plone-toolbar-state-title").click()
    context.driver.find_element(By.ID, "workflow-transition-reject").click()


@when(u'Producent is on page with all methods')
def step_impl(context):
    context.driver.find_element(By.LINK_TEXT, "Methods").click()


@then(u'he can see this method on page with all methods')
def step_impl(context):
    bodyText = context.driver.find_element_by_tag_name('body').text
    assert ("Combinatorial Testing Editing" in bodyText)

#Scenario: Consument cannot see private method
@given(u'producent logs out')
def step_impl(context):
    context.driver.find_element(By.CSS_SELECTOR, "#portal-personaltools span:nth-child(2)").click()
    context.driver.find_element(By.ID, "personaltools-logout").click()


@when(u'consument is on page with all methods')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.LINK_TEXT, "Methods").click()


@then(u'he cannot see the private method')
def step_impl(context):
    bodyText = context.driver.find_element_by_tag_name('body').text
    assert ("Combinatorial Testing Editing" not in bodyText)

#Scenario: Producent can see published method
@given(u'producent publishes the method')
def step_impl(context):
    context.driver.get("http://localhost:8080/VALU3S")
    context.driver.find_element(By.LINK_TEXT, "Methods").click()
    context.driver.find_element(By.LINK_TEXT, "Combinatorial Testing Editing").click()
    context.driver.find_element(By.CSS_SELECTOR, ".plone-toolbar-state-title").click()
    context.driver.find_element(By.ID, "workflow-transition-publish").click()
    