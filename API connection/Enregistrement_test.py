import sounddevice as sd
import scipy.io.wavfile
from google.cloud import storage

if __name__ == '__main__':

    fs = 44100
    duration = 5  # seconds
    myrecording = sd.rec(duration * fs, samplerate=fs, channels=2, dtype='float32') ### le met dans un numpy array
    print("Recording Audio")
    sd.wait()
    #print(myrecording)
    scipy.io.wavfile.write('enregistrement.wav', fs, myrecording) #ecrit un sound file wav

    # bucket_name = "your-bucket-name" ???
    # source_file_name = "local/path/to/file" --> enregistrement.wav
    # destination_blob_name = "storage-object-name" -->enregistrement2.wav

    storage_client =  storage.Client.from_service_account_json("SLIRUS_keyID.json")

    buckets = list(storage_client.list_buckets())
    print(buckets[0])

    bucket = storage_client.bucket("enregistrement_audio")
    blob = bucket.blob("Test_upload")
    blob.upload_from_filename('enregistrement.wav')



    #print(
     #   "File {} uploaded to {}.".format(
      #      source_file_name, destination_blob_name
      #  )
    #)



### code pour faire jouer lenregistrement
"""   
    print("Audio recording complete , Play Audio")
    sd.play(myrecording, fs)
    sd.wait()
    print("Play Audio Complete")"""
