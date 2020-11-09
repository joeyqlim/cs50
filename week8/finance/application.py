import os

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
    # Get info for each holding
    rows = db.execute("SELECT symbol, shares, price FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])
    portfolio = []
    totalValue = 0

    for info in rows:
        symbol = info["symbol"]
        shares = info["shares"]
        quote = lookup(symbol)
        name = quote["name"]
        price = quote["price"]
        value = shares * quote["price"]
        totalValue += value
        portfolio.append({'symbol': symbol.upper(), 'name': name, 'shares': shares, 'price': price, 'value': value})


    # Get cash balance
    result = db.execute("SELECT cash from users WHERE id=:id", id=session["user_id"])
    cash = result[0]["cash"]
    totalValue += cash

    return render_template("index.html", portfolio=portfolio, cash=cash, totalValue=totalValue)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        quote = lookup(symbol)
        name = quote["name"]
        price = quote["price"]
        cost = float(price) * float(shares)
        userId = session["user_id"]

        if symbol == "" or lookup(symbol) == None:
            return apology("must provide a valid symbol", 403)
        elif not int(shares) > 0:
            return apology("must provide number of shares", 403)

        # Query database for user's balance
        result = db.execute("SELECT cash from users WHERE id=:id", id=session["user_id"])
        cash = result[0]["cash"]

        # Check if user has enough cash
        if cost > cash:
            return apology("not enough cash", 403)

        # Update user's balance
        else:
            cash -= cost
            db.execute("""UPDATE users
                          SET cash = :cash
                          WHERE id = :id""",
                          cash=cash,
                          id=session["user_id"])

        # Update transactions table
        db.execute("INSERT INTO transactions (user_id, type, symbol, shares, price) VALUES(?, ?, ?, ?, ?)", userId, "buy", symbol, shares, price)

        return render_template("buy.html")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get info for each holding
    rows = db.execute("SELECT symbol, shares, price, date FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])
    history = []

    for info in rows:
        symbol = info["symbol"]
        shares = info["shares"]
        price = info["price"]
        date = info["date"]
        history.append({'symbol': symbol.upper(), 'shares': shares, 'price': price, 'date': date})

    return render_template("history.html", history=history)

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
    symbol = request.form.get("symbol")

    if request.method == "POST":
        quote = lookup(symbol)
        name = quote["name"]
        price = usd(quote["price"])

        return render_template("quoted.html", symbol=symbol, name=name, price=price)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        # Ensure passwords match
        elif not (request.form.get("password") == request.form.get("confirmation")):
            return apology("passwords do not match", 403)

        # Check if username exists in database
        usernameCount = db.execute("SELECT COUNT(*) FROM users WHERE username = :username",
                          username=request.form.get("username"))
        print(usernameCount)
        if usernameCount[0]["COUNT(*)"] != 0:
            return apology("username already exists", 403)

        # Insert user into database
        rows = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get info for each holding for select dropdown
    rows = db.execute("SELECT symbol, shares FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])
    portfolio = []

    for info in rows:
        symbol = info["symbol"]
        shares = info["shares"]
        portfolio.append({'symbol': symbol.upper(), 'shares': shares})

    # Get cash balance
    result = db.execute("SELECT cash from users WHERE id=:id", id=session["user_id"])
    cash = result[0]["cash"]

    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        soldShares = 0 - int(shares)
        quote = lookup(symbol)
        name = quote["name"]
        price = quote["price"]
        cost = float(price) * float(shares)
        userId = session["user_id"]

        if symbol == "" or lookup(symbol) == None:
            return apology("must provide a valid symbol", 403)
        elif not int(shares) > 0:
            return apology("must provide number of shares", 403)

        # Check if holding exists in portfolio
        shareCount = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id=:id AND symbol=:symbol",
                          id=session["user_id"], symbol=symbol)
        if shareCount[0]["SUM(shares)"] < 1:
            return apology("you do not own this stock", 403)

        # Update user's balance
        else:
            # Query database for user's balance
            result = db.execute("SELECT cash from users WHERE id=:id", id=session["user_id"])
            cash = result[0]["cash"]
            cash += cost
            db.execute("""UPDATE users
                          SET cash = :cash
                          WHERE id = :id""",
                          cash=cash,
                          id=session["user_id"])

        # Update transactions table
        db.execute("INSERT INTO transactions (user_id, type, symbol, shares, price) VALUES(?, ?, ?, ?, ?)", userId, "sell", symbol, soldShares, price)

        return render_template("sell.html")
    else:
        return render_template("sell.html", portfolio=portfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
