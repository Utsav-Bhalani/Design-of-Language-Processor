import csv
import time
import random
import string
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from webdriver_manager.chrome import ChromeDriverManager
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options # type: ignore

# Setup Chrome options
options = Options()
# options.add_argument("--headless")  # Uncomment to run in headless mode
options.add_argument("--incognito")
service = Service(ChromeDriverManager().install())

# Constants
SIGNUP_URL = "https://ai-based-evaluation-platform.onrender.com/signup"  # Replace with actual signup URL
LOGIN_URL = "https://ai-based-evaluation-platform.onrender.com/login"    # Replace with actual login URL
CSV_FILE = "users.csv"
ROLES = ["Faculty", "Student"]  # Only two roles now

# Function to generate a random string
def random_string(length=8):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

# Function to generate 15 random users
def generate_users():
    users = []
    for _ in range(15):
        name = random_string(6)
        email = f"{random_string(6)}@test.com"
        password = random_string(10)
        role = random.choice(ROLES)  # Only Faculty or Student
        users.append([name, email, password, role])

    # Save users to CSV file
    with open(CSV_FILE, "w", newline="") as file:
        writer = csv.writer(file)
        writer.writerow(["Name", "Email", "Password", "Role"])
        writer.writerows(users)

    print("✅ 15 users generated and saved to 'users.csv'")
    return users

# Function to perform signup
def signup_user(name, email, password, role):
    driver = webdriver.Chrome(service=service, options=options)
    driver.get(SIGNUP_URL)

    try:
        WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.NAME, "name")))

        # Fill form
        driver.find_element(By.NAME, "name").send_keys(name)
        driver.find_element(By.NAME, "email").send_keys(email)
        driver.find_element(By.NAME, "password").send_keys(password)

        # Select role
        role_dropdown = driver.find_element(By.NAME, "role")
        for option in role_dropdown.find_elements(By.TAG_NAME, "option"):
            if option.text == role:
                option.click()
                break

        # Submit form
        driver.find_element(By.CLASS_NAME, "auth-button").click()
        WebDriverWait(driver, 5).until(EC.url_changes(SIGNUP_URL))
        print(f"✅ Signed up: {email} as {role}")

    except Exception as e:
        print(f"❌ Signup failed for {email}: {e}")

    finally:
        driver.quit()

# Function to perform login
def login_user(email, password):
    driver = webdriver.Chrome(service=service, options=options)
    driver.get(LOGIN_URL)

    try:
        WebDriverWait(driver, 10).until(EC.presence_of_element_located((By.NAME, "email")))

        # Fill login form
        driver.find_element(By.NAME, "email").send_keys(email)
        driver.find_element(By.NAME, "password").send_keys(password)

        # Click login
        driver.find_element(By.CLASS_NAME, "auth-button").click()
        WebDriverWait(driver, 5).until(EC.url_changes(LOGIN_URL))
        print(f"✅ Logged in: {email}")

    except Exception as e:
        print(f"❌ Login failed for {email}: {e}")

    finally:
        driver.quit()

# Main execution
if __name__ == "__main__":
    users = generate_users()

    # Sequential signup
    for user in users:
        signup_user(*user)
        time.sleep(2)  # Pause to avoid rate limits

    print("\n🔹 All users signed up. Proceeding to login...\n")

    # Sequential login
    for user in users:
        _, email, password, _ = user  # Extract email & password
        login_user(email, password)
        time.sleep(2)  # Pause to avoid rate limits

    print("\n✅ All logins completed!")