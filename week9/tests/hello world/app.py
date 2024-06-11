from flask import Flask, render_template, request

appes = Flask(__name__)

@appes.route("/")
def index():
    name = request.args.get(key="name", default="world", type=str)
    return render_template("index.html", name=name)
