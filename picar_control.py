import  RPi.GPIO as GPIO
import firebase_admin 
from firebase_admin import credentials, db
GPIO.setmode(GPIO.BCM)
# cred = credentials.Certificate("/home/biswa/firebase.json")
# firebase_admin.initialize_app(cred,{
#     'databaseURL': ''
# })

# motor_pins = {}
