import smtplib
import os
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from dotenv import load_dotenv

# load environment variables from .env file
load_dotenv()

# config server 
SMTP_SERVER = os.getenv("SMTP_SERVER")
SMTP_PORT = int(os.getenv("SMTP_PORT", 587))
SENDER_EMAIL = os.getenv("SENDER_EMAIL")
SENDER_PASSWORD = os.getenv("SENDER_PASSWORD") # Note: not your regular password
API_URL = os.getenv("API_URL")

# Validate essential email configuration and provide helpful warnings if missing
missing = []
if not SMTP_SERVER:
    missing.append('SMTP_SERVER')
if not SENDER_EMAIL:
    missing.append('SENDER_EMAIL')
if not SENDER_PASSWORD:
    missing.append('SENDER_PASSWORD')
if missing:
    print(f"[notifications] WARNING: missing environment variables: {', '.join(missing)}. Email sending will fail until configured.")

def generate_receipt_html(receipt_data, receipt_id):
    """
    Create HTML design for the receipt notification email.
    Style: Modern Financial Card (Blue/Grey Theme)
    """

    # API links for actions
    approve_link = f"{API_URL}/api/approve/{receipt_id}"
    reject_link = f"{API_URL}/reject_form/{receipt_id}"

    html = f"""
    <!DOCTYPE html>
    <html>
    <body style="font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif; background-color: #f4f7f6; margin: 0; padding: 0;">
        
        <div style="width: 100%; padding: 40px 0;">
            
            <div style="max-width: 600px; margin: 0 auto; background-color: #ffffff; border-radius: 12px; box-shadow: 0 4px 15px rgba(0,0,0,0.05); overflow: hidden;">
                
                <div style="background-color: #2c3e50; padding: 30px; text-align: center;">
                    <h2 style="color: #ffffff; margin: 0; font-size: 20px; font-weight: 500; letter-spacing: 0.5px;">APPROVAL REQUEST</h2>
                </div>

                <div style="padding: 30px 20px; text-align: center; border-bottom: 1px solid #eeeeee;">
                    <p style="color: #7f8c8d; font-size: 12px; text-transform: uppercase; letter-spacing: 1px; margin: 0 0 5px 0;">Total Amount</p>
                    <h1 style="color: #2c3e50; font-size: 42px; margin: 0; font-weight: 700;">${receipt_data.get('total_amount', '0.00')}</h1>
                    <p style="color: #95a5a6; font-size: 14px; margin-top: 5px;">ID #{receipt_data.get('invoice_number', 'N/A')}</p>
                </div>

                <div style="padding: 20px 40px;">
                    <table style="width: 100%; border-collapse: separate; border-spacing: 0 10px;">
                        <tr>
                            <td style="color: #7f8c8d; font-size: 14px;">Provider</td>
                            <td style="text-align: right; color: #333; font-weight: 600; font-size: 14px;">{receipt_data.get('provider', 'Unknown Vendor')}</td>
                        </tr>
                        <tr>
                            <td style="color: #7f8c8d; font-size: 14px;">Issue Date</td>
                            <td style="text-align: right; color: #333; font-weight: 600; font-size: 14px;">{receipt_data.get('issue_date', 'N/A')}</td>
                        </tr>
                        <tr>
                            <td style="color: #7f8c8d; font-size: 14px;">System Status</td>
                            <td style="text-align: right; color: #f39c12; font-weight: 600; font-size: 14px;">Pending Review</td>
                        </tr>
                    </table>
                </div>

                <div style="padding: 10px 40px 40px; text-align: center;">
                    <a href="{approve_link}" style="display: inline-block; background-color: #27ae60; color: #ffffff; padding: 14px 30px; text-decoration: none; border-radius: 50px; font-weight: bold; font-size: 14px; margin: 5px; box-shadow: 0 3px 6px rgba(39, 174, 96, 0.2);">
                        Authorize Payment
                    </a>
                    <a href="{reject_link}" style="display: inline-block; background-color: #ffffff; color: #e74c3c; border: 1px solid #e74c3c; padding: 13px 29px; text-decoration: none; border-radius: 50px; font-weight: bold; font-size: 14px; margin: 5px;">
                        Reject
                    </a>
                </div>

                <div style="background-color: #f9f9f9; padding: 15px; text-align: center; border-top: 1px solid #eeeeee;">
                    <p style="font-size: 11px; color: #bdc3c7; margin: 0;">
                        Secure Notification | Ref ID: {receipt_id}
                    </p>
                </div>

            </div>
            
            <p style="text-align: center; color: #bdc3c7; font-size: 11px; margin-top: 20px;">
                &copy; Smart Receipt Processor AI
            </p>
        </div>
    </body>
    </html>
    """
    return html

def send_notification(destiny, receipt_data, receipt_id):
    """Send email notification about new receipt."""

    # Basic validation to give clearer error messages before attempting SMTP
    if not destiny:
        print("[notifications] WARNING: destination email is None. Provide MANAGER_EMAIL or pass a valid address.")
        return False
    if not SMTP_SERVER or not SENDER_EMAIL or not SENDER_PASSWORD:
        print("[notifications] WARNING: SMTP configuration incomplete. Check SMTP_SERVER, SENDER_EMAIL and SENDER_PASSWORD.")
        return False

    # create message
    msg = MIMEMultipart()
    msg["From"] = SENDER_EMAIL
    msg["To"] = destiny
    msg['Subject'] = f"Required action: Receipt {receipt_data.get('receipt_no', '---')}"

    # generate HTML content
    html_body = generate_receipt_html(receipt_data, receipt_id)
    msg.attach(MIMEText(html_body, 'html'))

    try: 

        server = smtplib.SMTP(SMTP_SERVER, SMTP_PORT)
        server.starttls()
        server.login(SENDER_EMAIL, SENDER_PASSWORD)
        server.send_message(msg)
        server.quit()
        print(f" Mail sent successfully to {destiny} regarding Receipt ID: {receipt_id}")
        return True
    except Exception as e:
        print(f" Failed to send email to {destiny}: {e}")
        return False
    
# test block

if __name__ == "__main__":

    # mock data
    sample_receipt = {
        "provider": "ACME Supplies S.A.",
        "receipt_no": "F123-456",
        "date": "15/09/2023",
        "total_amount": "1,250.75"
    }

    send_notification(os.getenv("SENDER_EMAIL"), sample_receipt, 1)