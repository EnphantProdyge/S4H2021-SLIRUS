#This file tests the API call
#google storage, m4a et flac,
#https://console.cloud.google.com/storage/browser/_details/enregistrement_audio/Enregistrement.flac?project=disco-alchemy-302319

#Imports
import requests
import io
import os
from google.cloud import speech
#from google.cloud.speech import enums
#from google.cloud.speech import types

#Main
if __name__ == '__main__':
    # Instantiates a client
    client = speech.SpeechClient.from_service_account_file("SLIRUS_keyID.json")


    #The name of the audio file to transcribe
    #file_name = "C:/Users/Raphael/Documents/Udes/S4/Projet/python programs/Enregistrement.flac"
    gcs_uri = "gs://enregistrement_audio/Enregistrement.flac"


    #Loads the audio into memory
    """with io.open(file_name, 'rb') as audio_file:
        content = audio_file.read()
        audio = speech.RecognitionAudio(content=content)"""

    audio = speech.RecognitionAudio(uri=gcs_uri)

    config = speech.RecognitionConfig(
        encoding=speech.RecognitionConfig.AudioEncoding.FLAC,
        audio_channel_count=2,
        sample_rate_hertz=48000,
        language_code="fr-CA",
    )

    response = client.recognize(config=config, audio=audio)

    for result in response.results:
        print("Transcript: {}".format(result.alternatives[0].transcript))