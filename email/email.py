import smtplib 
from email.message import EmailMessage 

email_subject = "Email test from Python Yoooo" 
sender_email_address = "areli1308@gmail.com" 
receiver_email_address = "arojoh@cua.uam.mx" 
email_smtp = "smtp.gmail.com" 
email_password = "hqkrtgcdvqejkuqe" #Para la contraseña en el caso de gmail es necesario generar una contraseña de aplicacion dentro de la 
                                    #cuenta de gmail de otro modo se genera error y no envia el correo esto debido a los protocolos de
                                    #seguridad de dos pasos q maneja Gmail

# Create an email message object 
message = EmailMessage() 

# Configure email headers 
message['Subject'] = email_subject 
message['From'] = sender_email_address 
message['To'] = receiver_email_address 

# Set email body text 
message.set_content("Hello from Python Areli!") 

# Set smtp server and port 
server = smtplib.SMTP(email_smtp, '587') 

# Identify this client to the SMTP server 
server.ehlo() 

# Secure the SMTP connection 
server.starttls() 

# Login to email account 
server.login(sender_email_address, email_password) 

# Send email 
server.send_message(message) 

# Close connection to server 
server.quit()