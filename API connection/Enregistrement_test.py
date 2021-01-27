import sounddevice as sd
from google.cloud import storage
from google.cloud import speech
import soundfile as sf

# Punctuations to remove in texts
PONC = ["!", '"', "'", ")", "(", ",", ".", ";", ":", "?", "-", "_", "\r", "]", "[", "«", "»", "’"]

def Speech_to_text():

    fs = 44100  # frame per second
    duration = 5  # seconds
    myrecording = sd.rec(duration * fs, samplerate=fs, channels=2, dtype='float32')  # Put recording in numpy array
    print("Recording Audio")
    sd.wait()  # Stop the recording
    # print(myrecording)

    sf.write('enregistrement.flac', myrecording, fs)  #Write the recording in flac file

    storage_client = storage.Client.from_service_account_json("SLIRUS_keyID.json") #Connection to google cloud

    #buckets = list(storage_client.list_buckets())
    #print(buckets[0])

    bucket = storage_client.bucket("enregistrement_audio")  # Go in the enregistrement_audio bucket
    blob = bucket.blob("Test_upload")  # Name of the audio file in google cloud storage
    blob.upload_from_filename('enregistrement.flac')  # Upload audio file in computer


### code pour faire jouer lenregistrement
    """   
    print("Audio recording complete , Play Audio")
    sd.play(myrecording, fs)
    sd.wait()
    print("Play Audio Complete")"""

# Instantiates a client
    client = speech.SpeechClient.from_service_account_file("SLIRUS_keyID.json")

#The name of the audio file to transcribe
    #file_name = "C:/Users/Raphael/Documents/Udes/S4/Projet/python programs/Enregistrement.flac"
    gcs_uri = "gs://enregistrement_audio/Test_upload" #The name of the audio file to transcribe

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

    #print(string)
    i = 0

    liste = []

    for caracter in string:
        for ponc in PONC:
            if ponc == caracter:
                string = string.replace(caracter, " ")

        if caracter == "à":
            string = string.replace(caracter,"a")
        elif caracter == "é":
            string = string.replace(caracter,"e")
        elif caracter == "ô":
            string = string.replace(caracter, "o")
        elif caracter == "ù":
            string = string.replace(caracter,"u")
        elif caracter == "è":
            string = string.replace(caracter,"e")
        elif caracter == "â":
            string = string.replace(caracter, "a")
        elif caracter == "ê":
            string = string.replace(caracter,"e")
        elif caracter == "û":
            string = string.replace(caracter,"u")

        if caracter == " ":
            continue
        liste.append(caracter[:-1].split(None))
        #print(caracter, "placer dans la liste a la position", i)
        i+=1
    print(string)


if __name__ == '__main__':
    Speech_to_text()
