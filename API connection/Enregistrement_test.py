import sounddevice as sd
#import scipy.io.wavfile
from google.cloud import storage

import soundfile as sf

if __name__ == '__main__':

    fs = 44100
    duration = 2  # seconds
    myrecording = sd.rec(duration * fs, samplerate=fs, channels=2, dtype='float32') ### le met dans un numpy array
    print("Recording Audio")
    sd.wait()
    #print(myrecording)

    sf.write('enregistrement.flac', myrecording, fs)

    storage_client = storage.Client.from_service_account_json("SLIRUS_keyID.json")

    buckets = list(storage_client.list_buckets())
    print(buckets[0])

    bucket = storage_client.bucket("enregistrement_audio")
    blob = bucket.blob("Test_upload")
    blob.upload_from_filename('enregistrement.flac')



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
