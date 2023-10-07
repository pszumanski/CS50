import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    for stock in db.execute(
        "SELECT * FROM stocks WHERE user_id=?;", session["user_id"]
    ):
        db.execute(
            "UPDATE stocks SET price=? WHERE stock=? AND user_id=?;",
            lookup(stock["stock"])["price"],
            stock["stock"],
            session["user_id"],
        )
        db.execute(
            "UPDATE stocks SET value=? WHERE stock=? AND user_id=?;",
            lookup(stock["stock"])["price"] * stock["shares"],
            stock["stock"],
            session["user_id"],
        )
    try:
        total = usd(
            db.execute(
                "SELECT SUM(value) FROM stocks WHERE user_id=?;", session["user_id"]
            )[0]["SUM(value)"]
            + db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"])
    except TypeError:
        total = usd(db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"])

    return render_template(
        "index.html",
        stocks=db.execute("SELECT * FROM stocks WHERE user_id=? ORDER BY shares DESC;",session["user_id"]),
        username=db.execute("SELECT username FROM users WHERE id=?;", session["user_id"])[0]["username"],
        balance=usd(db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"]),
        total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not lookup(request.form.get("symbol")):
            return apology("invalid stock symbol", 400)
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be a positive integer", 400)
        if shares < 1:
            return apology("number of shares must be a positive integer", 400)
        price = lookup(request.form.get("symbol"))["price"]
        balance = db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])
        if price * shares > balance[0]["cash"]:
            return apology("not enough balance to finalise transaction", 400)
        db.execute(
            "INSERT INTO purchases (user_id, symbol, price, shares, cost) VALUES (?, ?, ?, ?, ?);",
            session["user_id"],
            request.form.get("symbol"),
            price,
            shares,
            price * shares,
        )
        db.execute(
            "UPDATE users SET cash=? WHERE id=?;",
            balance[0]["cash"] - (price * shares),
            session["user_id"],
        )
        if not db.execute(
            "SELECT * FROM stocks WHERE user_id=? AND stock=? AND NOT shares=0;",
            session["user_id"],
            request.form.get("symbol"),
        ):
            db.execute(
                "INSERT INTO stocks (user_id, stock, shares, price, value) VALUES (?, ?, ?, ?, ?);",
                session["user_id"],
                request.form.get("symbol"),
                shares,
                db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"],
                db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"] * shares,
            )
        else:
            db.execute(
                "UPDATE stocks SET shares= (shares + ?) WHERE user_id=? AND stock=?;",
                shares,
                session["user_id"],
                request.form.get("symbol"),
            )
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:
        total = usd(db.execute("SELECT SUM(value) FROM stocks WHERE user_id=?;", session["user_id"])[0]["SUM(value)"] + db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"])
    except TypeError:
        total = usd(db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"])
    return render_template("history.html",
                           history=db.execute("SELECT * FROM purchases WHERE user_id=? ORDER BY date DESC;",session["user_id"]),
                           username=db.execute("SELECT username FROM users WHERE id=?;", session["user_id"])[0]["username"],
                           balance=usd(db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0]["cash"]),
                           total=total)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 400)

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
        try:
            symbol = lookup(request.form.get("symbol"))["symbol"]
            return render_template(
                "quoted.html",
                symbol=symbol,
                price=lookup(request.form.get("symbol"))["price"])
        except TypeError:
            return apology("invalid symbol", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)
        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match, matching passwords required", 400)
        # Ensure username not in use
        for user in db.execute("SELECT username FROM users;"):
            if request.form.get("username") == user["username"]:
                return apology("username already in use", 400)
        # Register user
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?);", request.form.get("username"), generate_password_hash(request.form.get("password")))
        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Check if symbol is valid
        if not lookup(request.form.get("symbol")):
            return apology("invalid stock symbol", 400)
        # Check if stock is owned
        try:
            db.execute("SELECT * FROM stocks WHERE user_id=? AND stock=?;",session["user_id"], request.form.get("symbol"))[0]["stock"]
        except IndexError:
            return apology("stock not owned", 400)
        try:
            shares=int(request.form.get("shares"))
        except ValueError:
            return apology("number of shares must be a positive integer", 400)
        # Check if asked amount is valid
        if shares < 1:
            return apology("number of shares must be a positive integer", 400)
        # Check if enough stock owned
        if shares > db.execute("SELECT * FROM stocks WHERE user_id=? AND stock=?;",session["user_id"], request.form.get("symbol"))[0]["shares"]:
            return apology("that many stocks not owned", 400)
        # Check price
        price = lookup(request.form.get("symbol"))["price"]
        # Check balance
        balance = db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])
        # Save sell
        db.execute("INSERT INTO purchases (user_id, symbol, price, shares, cost) VALUES (?, ?, ?, ?, ?);", session["user_id"], request.form.get("symbol"), price, -shares, price * (-shares))
        # Add to balance
        db.execute("UPDATE users SET cash=? WHERE id=?;", balance[0]["cash"] + (price * shares), session["user_id"])
        # Decrease or remove stock
        if  db.execute("SELECT * FROM stocks WHERE user_id=? AND stock=? AND NOT shares=?", session["user_id"], request.form.get("symbol"), shares):
            db.execute("UPDATE stocks SET shares=? WHERE user_id=? AND stock=?;", db.execute("SELECT * FROM stocks WHERE user_id=? AND stock=?;",session["user_id"], request.form.get("symbol"))[0]["shares"] - shares, session["user_id"], request.form.get("symbol"))
        else:
            db.execute("DELETE FROM STOCKS WHERE user_id=? AND stock=?;", session["user_id"], request.form.get("symbol"))
        return redirect("/")
    else:
        return render_template("sell.html", stocks=db.execute("SELECT * FROM stocks WHERE user_id=?;", session["user_id"]))

# Allow user to change their password
@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    """Edit your account."""
    if request.method == "POST":
         # Load user from database
        rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

        # Ensure password given
        if not request.form.get("password"):
            return apology("must provide current password", 400)
        # Ensure password is correct
        elif not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid current password", 400)
        # Ensure new password given
        elif not request.form.get("newpassword"):
            return apology("must provide new password", 400)
        # Ensure passwords match
        elif request.form.get("newpassword") != request.form.get("confirmation"):
            return apology("new passwords do not match, matching passwords required", 400)
        # Update password
        db.execute("UPDATE users SET hash=?;", generate_password_hash(request.form.get("newpassword")))
        return redirect("/")
    else:
        return render_template(
                                "account.html",
                               username=db.execute(
                                    "SELECT username FROM users WHERE id=?;",
                                    session["user_id"])[0]["username"],
                                    )
