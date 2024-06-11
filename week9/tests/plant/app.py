from flask import Flask, render_template, request
import random
import os

app = Flask(__name__)

plant_count = 0

PLANT_IMAGES = [
    "plant1.png",
    "plant2.png",
    "plant3.png",
    "plant4.png",
    "plant5.png",
    "plant6.png",
    "plant7.png",
    "plant8.png",
]

DEAD_PLANT_IMAGES = [
    "dead-plant1.png",
]

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        return render_template("greet.html", name=request.form.get("name", ""))
    
    return render_template("index.html")

@app.route("/greet", methods=["POST"])
def greet():
    return render_template("greet.html", name=request.form.get("name", ""))

@app.route("/plant")
def load_plant():
    # Yeah this is buggy
    global plant_count
    plant_count = plant_count + 1
    
    image = get_plant_image()

    return render_template("plant.html", image=image, plant_count=plant_count)

@app.route("/reset", methods=["POST"])
def reset_plant_count():
    global plant_count

    if request.method == "POST":
        plant_count = 0

    image = get_plant_image()

    return render_template("plant.html", image=image, plant_count=plant_count)

def get_plant_image():
    global plant_count

    if plant_count > 4:
        return random.choice(DEAD_PLANT_IMAGES)
    else:
        return random.choice(PLANT_IMAGES)
    
if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=int(os.environ.get('PORT', 8080)))