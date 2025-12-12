import smtplib
from email.mime.text import MIMEText
from datetime import datetime
import configparser
import logging

# Read configuration file
config = configparser.ConfigParser()
config.read("config.ini")

# Email credentials from config file
SMTP_SERVER = config["EMAIL"]["smtp_server"]
SMTP_PORT = config["EMAIL"]["smtp_port"]
EMAIL_ADDRESS = config["EMAIL"]["email_address"]
EMAIL_PASSWORD = config["EMAIL"]["email_password"]

# Set up logging
logging.basicConfig(
    filename="birthday_sms.log", level=logging.INFO, format="%(asctime)s - %(message)s"
)

# Recipient information with carrier's email-to-SMS gateway domain
birthdays = [
    {"name": "Test", "birthday": "12-18", "phone": "4404035929", "carrier": "tmobile"},
    {"name": "Dima", "birthday": "06-06", "phone": "4404035929", "carrier": "tmobile"},
    {"name": "Tommy", "birthday": "02-23", "phone": "2156794031", "carrier": ""},
    {"name": "Matt", "birthday": "03-28", "phone": "9372609108", "carrier": ""},
    {"name": "Abby", "birthday": "04-05", "phone": "3304015774", "carrier": "tmobile"},
    {"name": "Mom", "birthday": "04-10", "phone": "4409905629", "carrier": "tmobile"},
    {"name": "Emma Parrish", "birthday": "04-15", "phone": "2164035473", "carrier": ""},
    {"name": "Isaac", "birthday": "04-24", "phone": "2104403542", "carrier": ""},
    {
        "name": "Adrian Dimalan",
        "birthday": "04-27",
        "phone": "2165664106",
        "carrier": "",
    },
    {"name": "Josh", "birthday": "05-17", "phone": "3303409916", "carrier": "tmobile"},
    {
        "name": "Aunt Beckey",
        "birthday": "05-22",
        "phone": "4698775009",
        "carrier": "tmobile",
    },
    {
        "name": "Aunt Rachel",
        "birthday": "5-27",
        "phone": "3303409786",
        "carrier": "tmobile",
    },
    {
        "name": "Aunt Megan",
        "birthday": "04-10",
        "phone": "6143144638",
        "carrier": "tmobile",
    },
    {
        "name": "Brianna",
        "birthday": "06-09",
        "phone": "3304012141",
        "carrier": "tmobile",
    },
    {"name": "Jackson", "birthday": "06-29", "phone": "4198067024", "carrier": ""},
    {"name": "Ethan", "birthday": "07-01", "phone": "7177360474", "carrier": ""},
    {"name": "Shura", "birthday": "07-10", "phone": "2168045407", "carrier": "tmobile"},
    {"name": "Dad", "birthday": "07-18", "phone": "2162157925", "carrier": "tmobile"},
    # {"name": "Neil", "birthday": "08-07", "phone": "", "carrier": "tmobile"},
    {"name": "Levi", "birthday": "08-12", "phone": "4403053472", "carrier": ""},
    {"name": "Clay", "birthday": "08-14", "phone": "6145783251", "carrier": "tmobile"},
    {"name": "Michael Ta", "birthday": "08-15", "phone": "4405323282", "carrier": ""},
    {"name": "Lukas", "birthday": "08-15", "phone": "2164075141", "carrier": ""},
    {"name": "Jake", "birthday": "08-15", "phone": "4173373438", "carrier": "tmobile"},
    {
        "name": "Grandpa",
        "birthday": "08-27",
        "phone": "6143145245",
        "carrier": "tmobile",
    },
    {"name": "Luke", "birthday": "08-29", "phone": "4405321379", "carrier": ""},
    {"name": "Alex", "birthday": "09-10", "phone": "2168981540", "carrier": ""},
    {"name": "Jeb", "birthday": "09-17", "phone": "2817310367", "carrier": "tmobile"},
    {
        "name": "Andrew Sperry",
        "birthday": "10-15",
        "phone": "4404299257",
        "carrier": "",
    },
    # {"name": "Tolik", "birthday": "12-06", "phone": "", "carrier": "tmobile"},
]

# Carrier gateways for email-to-SMS
CARRIER_GATEWAYS = {
    "verizon": "vtext.com",  # For Verizon
    "att": "txt.att.net",  # For AT&T
    "tmobile": "tmomail.net",  # For T-Mobile
    "sprint": "messaging.sprintpcs.com",  # For Sprint
}


def send_email_sms(to, message):
    """Send SMS using an email-to-SMS gateway."""
    try:
        # Connect to the SMTP server
        with smtplib.SMTP(SMTP_SERVER, SMTP_PORT) as server:
            server.starttls()
            server.login(EMAIL_ADDRESS, EMAIL_PASSWORD)

            # Create the message
            msg = MIMEText(message)
            msg["From"] = EMAIL_ADDRESS
            msg["To"] = to
            msg["Subject"] = "Happy Birthday"

            # Send the message
            server.sendmail(EMAIL_ADDRESS, to, msg.as_string())
            print(f"Message sent to {to}")
    except Exception as e:
        print(f"Failed to send message to {to}: {e}")


def send_happy_birthday():
    """Check for birthdays and send SMS."""
    today = datetime.now().strftime("%m-%d")
    for person in birthdays:
        if person["birthday"] == today:
            message = f"{person['name']}. Congrats on leveling up."
            sent = False
            carriers_to_try = [person["carrier"]] if person["carrier"] else []
            carriers_to_try += [
                carrier
                for carrier in CARRIER_GATEWAYS
                if carrier not in carriers_to_try
            ]

            for carrier in carriers_to_try:
                gateway = CARRIER_GATEWAYS.get(carrier, "")
                if not gateway:
                    continue
                sms_address = f"{person['phone']}@{gateway}"
                try:
                    send_email_sms(sms_address, message)
                    sent = True
                    person["carrier"] = carrier  # Update carrier on success
                    logging.info(f"Message sent to {person['name']} via {carrier}.")
                    print(f"Message sent to {person['name']} via {carrier}.")
                    break
                except Exception as e:
                    print(f"Failed to send message via {carrier}: {e}")
            if not sent:
                print(f"Failed to send message to {person['name']} via all carriers.")
        else:
            print(f"No birthday today.")


print("Running test for Birthday SMS service...")
send_happy_birthday()
