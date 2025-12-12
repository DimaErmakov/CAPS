import os
import re
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from bs4 import BeautifulSoup

# If modifying these SCOPES, delete the file token.json.
SCOPES = ["https://www.googleapis.com/auth/gmail.readonly"]


def authenticate_gmail():
    """Authenticate and return the Gmail API service."""
    creds = None
    if os.path.exists("token.json"):
        creds = Credentials.from_authorized_user_file("token.json", SCOPES)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file("credentials.json", SCOPES)
            creds = flow.run_local_server(port=0)
        with open("token.json", "w") as token:
            token.write(creds.to_json())
    return build("gmail", "v1", credentials=creds)


def search_emails(service, query):
    """Search for emails matching the query."""
    results = service.users().messages().list(userId="me", q=query).execute()
    messages = results.get("messages", [])
    return messages


def get_email_content(service, msg_id):
    """Get the content of the email."""
    msg = (
        service.users().messages().get(userId="me", id=msg_id, format="full").execute()
    )
    return msg


def extract_links_from_email(email_content):
    """Extract links from the email body."""
    links = []
    if "parts" in email_content["payload"]:
        for part in email_content["payload"]["parts"]:
            if part["mimeType"] == "text/html":
                body = part["body"]["data"]
                body = body.replace("-", "+").replace("_", "/")
                decoded_body = base64.urlsafe_b64decode(body).decode("utf-8")
                soup = BeautifulSoup(decoded_body, "html.parser")
                for link in soup.find_all("a", href=True):
                    if "qualtrics" in link["href"]:
                        links.append(link["href"])
    return links


def main():
    service = authenticate_gmail()
    query = 'subject:"S.A.O. Newsletter"'
    emails = search_emails(service, query)

    if not emails:
        print("No emails found with the subject 'S.A.O. Newsletter'.")
        return

    for email in emails:
        email_content = get_email_content(service, email["id"])
        links = extract_links_from_email(email_content)
        if links:
            print(f"Links found in email {email['id']}:")
            for link in links:
                print(link)
        else:
            print(f"No Qualtrics links found in email {email['id']}.")


if __name__ == "__main__":
    main()
