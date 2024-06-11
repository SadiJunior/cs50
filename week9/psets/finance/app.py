import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, is_float

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Max amount of cash to be deposited
MAX_DEPOSIT = 10000.00


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = get_user_id()

    stocks = db.execute(
        "SELECT symbol, shares FROM stocks WHERE user_id = ?", user_id
    )

    total_stock_value = 0

    for stock in stocks:

        # Adds aditional information to each stock from stocks dictionary
        current_price = lookup(stock["symbol"])["price"]
        stock_value = current_price * stock["shares"]
        stock["current_price"] = current_price
        stock["stock_value"] = stock_value

        # Keeps the sum of all stocks
        total_stock_value += stock_value

    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    total_cash = total_stock_value + user_cash

    return render_template("index.html", stocks=stocks, user_cash=user_cash, total_cash=total_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        form_symbol = request.form.get("symbol")
        num_of_shares_text = request.form.get("shares")

        if not form_symbol:
            return apology("must provide symbol", 400)

        if not num_of_shares_text:
            return apology("must provide number of shares", 400)

        if not num_of_shares_text.isnumeric():
            return apology("must inform a number, not text", 400)

        num_of_shares = int(num_of_shares_text)

        if num_of_shares < 1:
            return apology("invalid number of shares", 400)

        data = lookup(form_symbol)

        if not data:
            return apology("invalid symbol")

        user_id = get_user_id()

        price = data["price"]
        total_price = price * num_of_shares

        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        if total_price > user_cash:
            return apology("not enough cash", 403)

        symbol = data["symbol"]

        user_stock_data = db.execute(
            "SELECT * FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol
        )

        try:
            if user_stock_data:
                user_stock = user_stock_data[0]
                db.execute(
                    "UPDATE stocks SET shares = shares + ? WHERE id = ?", num_of_shares, user_stock["id"]
                )

            else:
                # Adds the data of the stock bought
                db.execute(
                    "INSERT INTO stocks (user_id, symbol, shares) VALUES (?, ?, ?)", user_id, symbol, num_of_shares
                )

            # Updates the User cash
            new_user_cash = user_cash - total_price
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?", new_user_cash, user_id
            )

            db.execute(
                "INSERT INTO transactions (user_id, operation, symbol, shares, price, transaction_cash) VALUES (?, ?, ?, ?, ?, ?)", user_id, "buy", symbol, num_of_shares, price, total_price
            )
        except:
            return apology("internal error when buying stocks", 500)

        flash("Bougth!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    user_id = get_user_id()

    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = ?", user_id
    )

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        form_symbol = request.form.get("symbol")

        if not form_symbol:
            return apology("must provide symbol", 400)

        data = lookup(form_symbol)

        if not data:
            return apology("invalid symbol")

        return render_template("quoted.html", price=data["price"], symbol=data["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        password_confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not password_confirmation:
            return apology("must provide password confirmation", 400)

        elif password != password_confirmation:
            return apology("password differs from password confirmation", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 0:
            return apology(f"username {username} is already being used", 400)

        password_hash = generate_password_hash(password)

        try:
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", username, password_hash
            )
        except:
            return apology("internal error when registering", 500)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = get_user_id()

    shares = db.execute(
        "SELECT symbol, shares FROM stocks WHERE user_id = ?", user_id
    )

    shares_by_symbols = {share["symbol"]: share["shares"] for share in shares}

    symbols = shares_by_symbols.keys()

    if request.method == "POST":

        symbol_form = request.form.get("symbol")
        num_of_shares_text = request.form.get("shares")

        if not symbol_form:
            return apology("must provide a symbol", 400)

        if symbol_form not in symbols:
            return apology("you don't own any shares of this symbol", 400)

        if not num_of_shares_text:
            return apology("must provide number of shares", 400)

        if not num_of_shares_text.isnumeric():
            return apology("must provide a number, not text", 400)

        data = lookup(symbol_form)

        if not data:
            return apology("invalid symbol")

        symbol = data["symbol"]

        num_of_shares = int(num_of_shares_text)
        total_shares = shares_by_symbols[symbol]

        if num_of_shares < 1 or num_of_shares > total_shares:
            return apology("invalid number of shares", 400)

        price = data["price"]
        total_value = price * num_of_shares

        try:
            db.execute(
                "UPDATE users SET cash = cash + ? WHERE id = ?", total_value, user_id
            )

            db.execute(
                "INSERT INTO transactions (user_id, operation, symbol, shares, price, transaction_cash) VALUES (?, ?, ?, ?, ?, ?)", user_id, "sell", symbol, num_of_shares, price, total_value
            )

            if num_of_shares >= total_shares:
                db.execute(
                    "DELETE FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol
                )
            else:
                db.execute(
                    "UPDATE stocks SET shares = shares - ? WHERE user_id = ? AND symbol = ?", num_of_shares, user_id, symbol
                )
        except:
            return apology("internal error when selling stocks", 500)

        flash("Sold!")
        return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)


@app.route("/user")
@login_required
def user():
    """Display user data"""

    user_id = get_user_id()

    user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    return render_template("user.html", user_data=user_data, max_deposit=MAX_DEPOSIT)


@app.route("/user/deposit", methods=["POST"])
@login_required
def user_deposit():
    """Deposit user cash (not real money)"""

    amount_text = request.form.get("amount")

    if not amount_text:
        return apology("must provide deposit amount", 400)

    if not is_float(amount_text):
        return apology("must provide a valid number", 400)

    amount = round(float(amount_text), 2)

    if amount < 0:
        return apology("must provide positive amount", 400)

    if amount > MAX_DEPOSIT:
        return apology(f"limit of deposit is {usd(MAX_DEPOSIT)}", 400)

    user_id = get_user_id()

    try:
        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?", amount, user_id
        )

        db.execute(
            "INSERT INTO transactions (user_id, operation, transaction_cash) VALUES (?, ?, ?)", user_id, "deposit", amount
        )

    except:
        return apology("internal error when depositing", 500)

    flash("Deposited!")

    return redirect("/user")


@app.route("/user/update", methods=["POST"])
@login_required
def user_update():
    """Display/update user data"""

    user_id = get_user_id()

    user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)[0]

    new_username = request.form.get("new_username")

    current_password = request.form.get("current_password")
    new_password = request.form.get("new_password")
    confirmation = request.form.get("confirmation")

    if not (new_username or current_password or new_password or confirmation):
        return apology("no user data specified", 400)

    if new_username:
        error_message, error_code = update_username(new_username)

        if error_message and error_code:
            return apology(error_message, error_code)

        flash("Updated username!")

    if current_password or new_password or confirmation:
        error_message, error_code = update_password(current_password, new_password, confirmation)

        if error_message and error_code:
            return apology(error_message, error_code)

        flash("Updated password!")

    return redirect("/user")


def update_username(new_username):
    if not new_username:
        return "invalid username", 400

    user_id = get_user_id()

    user_data = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]

    current_username = user_data["username"]

    if new_username == current_username:
        return "new username is equal to current one", 400

    rows = db.execute("SELECT * FROM users WHERE username = ?", new_username)

    if len(rows) >= 1:
        return f"username {new_username} is already being used", 400

    try:
        db.execute(
            "UPDATE users SET username = ? WHERE id = ?", new_username, user_id
        )
    except:
        return "internal error when updating username", 500

    return "", 0


def update_password(current_password, new_password, confirmation):
    if not current_password:
        return "must inform current password", 400

    if not new_password:
        return "must provide new password", 400

    if not confirmation:
        return "must provide password confirmation", 400

    if new_password != confirmation:
        return "passwords don't match", 400

    user_id = get_user_id()

    user_data = db.execute("SELECT hash FROM users WHERE id = ?", user_id)[0]

    current_hash = user_data["hash"]

    if not check_password_hash(current_hash, current_password):
        return "wrong password", 403

    if check_password_hash(current_hash, new_password):
        return "new password is equal to current one", 400

    new_hash = generate_password_hash(new_password)

    try:
        db.execute(
            "UPDATE users SET hash = ? WHERE id = ?", new_hash, user_id
        )
    except:
        return "internal error when updating password", 500

    return "", 0


def get_user_id():
    return session["user_id"]
