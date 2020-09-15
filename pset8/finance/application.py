import os
import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    username = db.execute("SELECT username FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["username"]
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]
    tradeDetails = db.execute("SELECT * FROM :username ORDER BY nos DESC", username = username)

    return render_template("index.html", details = tradeDetails, cash = cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")
    else:

        rows = db.execute("SELECT * FROM users WHERE id = :user_id", user_id = session["user_id"])
        username = rows[0]["username"]
        cash = int(rows[0]["cash"])
        symbol = request.form.get("symbol")
        quantity = int(request.form.get("quantity"))

        data = lookup(symbol)

        shareAmount = quantity * int(data["price"])


        if cash < shareAmount:
            return apology("You don't have sufficient balance")

        balance = cash - shareAmount
        time = datetime.datetime.now()
        db.execute("INSERT INTO history (username, symbol, qty, price, transacted) VALUES (:username, :s, :q, :p, :t)", username = username, s = symbol, q = quantity, p = data["price"], t = time)
        db.execute("INSERT INTO :username (symbol, name, qty, action, price, value) VALUES (:s, :n, :q, :ac, :p, :v)", username = username, s = symbol, n = data["name"], q = quantity, ac = "BUY", p = data["price"], v = shareAmount)
        db.execute("UPDATE users SET cash = :balance WHERE username = :username", balance = balance, username = username)

        flash("Bought")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    username = db.execute("SELECT username FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["username"]
    history = db.execute("SELECT * FROM history WHERE username = :username ORDER BY nos ASC", username = username)

    return render_template("history.html", history = history)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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

    if request.method == "GET":
        return render_template("quote.html")

    else:
        symbol = request.form.get("symbol")
        data = lookup(symbol)

        if data == None:
            return apology("Entered symbol is incorrect!")
        else:
            return render_template("quoteOutput.html", data = data)



@app.route("/register", methods=["GET", "POST"])
def register():
    """ Register user """

    # return register.html if GET
    if request.method == "GET":
        return render_template("register.html")

    else:
        # getting password from form
        password = request.form.get("password")
        checkPassword = request.form.get("checkPassword")

        if password != checkPassword:
            return apology("Password entered not correct!")

        username = request.form.get("username")
        if username == None:
            return apology("Username not entered!")

        # get usernames from the database
        fromdb = db.execute("SELECT username FROM users where username = :username", username = username)

        # checking whether the user exists or not
        if len(fromdb) != 0:
            return apology("User already exists!")

        # entering data into the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashPassword)", username = username, hashPassword = generate_password_hash(password))
        db.execute("CREATE TABLE history ('nos' INTEGER NOT NULL PRIMARY KEY, 'username' TEXT NOT NULL, 'symbol' TEXT NOT NULL, 'qty' INTEGER, 'price' INTEGER, 'transacted' DATETIME)")
        db.execute("CREATE TABLE :username ('nos' INTEGER NOT NULL PRIMARY KEY, 'symbol' TEXT NOT NULL, 'name' TEXT NOT NULL, 'qty' INTEGER, 'action' TEXT, 'price' INTEGER, 'value' INTEGER)", username = username)

        flash("Registered")

        # redirecting to the login page for loging in
        return redirect("/login")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        username = db.execute("SELECT username FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["username"]
        tradeDetails = db.execute("SELECT symbol, SUM(qty) FROM :username", username = username)
        return render_template("sell.html", shares = tradeDetails)

    else:
        username = db.execute("SELECT username FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["username"]
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id = session["user_id"])[0]["cash"]


        symbol = request.form.get("symbol")
        quantity = int(request.form.get("quantity"))

        userData = db.execute("SELECT * FROM :username WHERE symbol = :symbol", username = username, symbol = symbol)

        data = lookup(symbol)

        currentValue = quantity * data["price"]
        balance = cash + currentValue

        newQuantity = int(userData[0]["qty"]) - quantity
        newValue = newQuantity * int(data["price"])
        time = datetime.datetime.now()
        db.execute("INSERT INTO history (username, symbol, qty, price, transacted) VALUES (:username, :s, :q, :p, :t)", username = username, s = symbol, q = -1 *quantity, p = data["price"], t = time)
        db.execute("UPDATE :username SET qty = :q, action = :ac, price = :p, value = :v WHERE symbol = :s", username = username, s = symbol, q = newQuantity, ac = "SELL", p = data["price"], v = newValue)
        db.execute("UPDATE users SET cash = :balance WHERE username = :username", balance = balance, username = username)

        flash("Sold")

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
