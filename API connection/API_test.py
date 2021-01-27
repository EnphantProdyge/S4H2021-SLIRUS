#This file tests the API call
#google storage, m4a et flac,
#https://console.cloud.google.com/storage/browser/_details/enregistrement_audio/Enregistrement.flac?project=disco-alchemy-302319

#Imports
from google.cloud import speech



#Main
if __name__ == '__main__':
    # Instantiates a client
    client = speech.SpeechClient.from_service_account_file("SLIRUS_keyID.json")

    gcs_uri = "gs://enregistrement_audio/Test_upload"


    #Loads the audio into memory

    audio = speech.RecognitionAudio(uri=gcs_uri)

    config = speech.RecognitionConfig(
        audio_channel_count=2,
        sample_rate_hertz=44100,
        language_code="fr-CA",
    )

    response = client.recognize(config=config, audio=audio)



    for result in response.results:
        #print(format(result.alternatives[0].transcript))
        string = format(result.alternatives[0].transcript)
        #list[i] = format(result.alternatives[0].transcript)

    print(string)
    i = 0

    liste = []

    for caracter in string:
        if caracter == " ":
            continue
        liste.append(caracter[:-1].split(None))
        print(caracter, "placer dans la liste a la position", i)
        i+=1

    print(liste[4])