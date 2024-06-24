from mediapipe_model_maker import gesture_recognizer
import os
import tensorflow as tf

IMAGES_PATH = "../Imagenes"

if os.path.exists(IMAGES_PATH):
    print("El archivo fue encontrado en la ruta especificada.")
    print(IMAGES_PATH)
    labels = []
    for i in os.listdir(IMAGES_PATH):
        if os.path.isdir(os.path.join(IMAGES_PATH, i)):
            labels.append(i)
    print(labels)
else:
    print("El archivo no fue encontrado en la ruta especificada.")

data = gesture_recognizer.Dataset.from_folder(
    dirname=IMAGES_PATH,
    hparams=gesture_recognizer.HandDataPreprocessingParams()
)

train_data, rest_data = data.split(0.8)
validation_data, test_data = rest_data.split(0.5)

hparams = gesture_recognizer.HParams(export_dir="lsm_model")
options = gesture_recognizer.GestureRecognizerOptions(hparams=hparams)
model = gesture_recognizer.GestureRecognizer.create(
    train_data=train_data,
    validation_data=validation_data,
    options=options
)

loss, acc = model.evaluate(test_data, batch_size=1)
print(f"Test loss:{loss}, Test accuracy:{acc}")

model.export_model()