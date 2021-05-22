from selenium import webdriver
from selenium.common.exceptions import WebDriverException
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

BASE_URL = "http://localhost:8080/VALU3S"

def get_webdriver():
	try:
		driver = webdriver.Remote(
			command_executor='http://localhost:4444/wd/hub',
			desired_capabilities=DesiredCapabilities.CHROME)
	except WebDriverException:
			driver = webdriver.Remote(
				command_executor='http://localhost:4444/wd/hub',
				desired_capabilities=DesiredCapabilities.FIREFOX)
	driver.implicitly_wait(5)
	return driver

def before_all(context):
	context.driver = get_webdriver()

def after_all(context):
	context.driver.quit()