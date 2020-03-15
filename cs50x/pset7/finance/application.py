import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    # Get username and available cash
    user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
    username = user[0]["username"]
    available_cash = user[0]["cash"]
    grand_total = available_cash

    # Get stocks owned
    stocks = db.execute(
        "SELECT total(count), symbol FROM transaction_history WHERE userId = :userId GROUP BY symbol", userId=session["user_id"])

    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        stock["total_cost"] = float(stock["price"]) * float(stock["total(count)"])
        grand_total = float(grand_total) + stock["total_cost"]
        stock["total(count)"] = int(stock["total(count)"])
        stock["total_cost"] = usd(stock["total_cost"])
        stock["price"] = usd(stock["price"])
    
    return render_template("index.html", username=username, available_cash=usd(available_cash), stocks=stocks, grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide positive number of shares", 400)
        
        # Reject if not string
        if not request.form.get("symbol"):
            return apology("invalid symbol", 400)
            
        try:
            num_shares = int(request.form.get("shares"))
            if num_shares < 0:
                return apology("must provide positive integer", 400)
        except ValueError:
            return apology("must provide integer", 400)
        
        quote = lookup(request.form.get("symbol"))
        if quote:
            cost = quote["price"] * float(request.form.get("shares"))
        else:
            return apology("Invalid symbol", 400)
        
        row = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        
        if float(row[0]["cash"]) < cost:
            return apology("not enough cash")
        else:
            # update transaction history
            db.execute("INSERT INTO transaction_history (userId, symbol, count, price, total_cost, action) VALUES (:userId, :symbol, :count, :price, :total_cost, 'buy')", userId=int(
                session["user_id"]), symbol=quote["symbol"], count=request.form.get("shares"), price=quote["price"], total_cost=cost)
            # calculate cash remaining after purchase
            cash = float(row[0]["cash"]) - cost
            # update user's cash
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id=session["user_id"])
            flash('Congrats!')
            return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    # Get transaction history
    transactions = db.execute("SELECT * FROM transaction_history WHERE userId = :userId", userId=session["user_id"])
    
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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash('You were successfully logged in.')
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
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Reject if not string
        if not symbol.isalpha():
            return apology("invalid symbol", 400)

        quote = lookup(symbol)
        if quote:
            quote["price"] = usd(quote["price"])
            return render_template("quoted.html", quote=quote)
        else:
            return apology("invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username does not already exist
        if len(rows) == 1:
            return apology("invalid username and/or password", 400)
            
        # Ensure passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Create user in db
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get(
            "username"), hash=generate_password_hash(request.form.get("password")))

        # Redirect user to success page
        return redirect("/register_success")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/register_success")
def register_success():
    return render_template("register_success.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure number of shares was submitted
        elif not request.form.get("shares"):
            return apology("must provide positive number of shares", 400)
        
        try:
            num_shares = int(request.form.get("shares"))
        except ValueError:
            return apology("must provide integer", 400)
        
        # Get stocks owned
        stocks = db.execute("SELECT total(count), symbol FROM transaction_history WHERE userId = :userId AND symbol = :symbol GROUP BY symbol = :symbol",
                            userId=session["user_id"], symbol=request.form.get("symbol"))
        
        error_message = "you don\'t own enough stocks of " + request.form.get("symbol")

        # throw error if user doesn't own stocks
        if not stocks:
            return apology(error_message)
        
        # Get username and available cash
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        username = user[0]["username"]
        available_cash = user[0]["cash"]

        # get latest price
        quote = lookup(request.form.get("symbol"))
        
        # throw error if don't own enough shares
        if float(request.form.get("shares")) > float(stocks[0]["total(count)"]):
            return apology(error_message, 400)

        if quote:
            # calculate cash earned
            cost = quote["price"]
            cash_earned = cost * float(request.form.get("shares"))

            # update transaction history
            db.execute("INSERT INTO transaction_history (userId, symbol, count, price, total_cost, action) VALUES (:userId, :symbol, :count, :price, :total_cost, 'sell')", userId=int(
                session["user_id"]), symbol=quote["symbol"], count=-int(request.form.get("shares")), price=quote["price"], total_cost=cost)

            # update user's cash
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=(available_cash + cash_earned), id=session["user_id"])

            flash('Congrats!')
            return redirect("/")

        else:
            return apology("Invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Get stocks owned
        stocks = db.execute(
            "SELECT total(count), symbol FROM transaction_history WHERE userId = :userId GROUP BY symbol", userId=session["user_id"])

        return render_template("sell.html", stocks=stocks)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Inject more funds"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure amount was submitted
        if not request.form.get("amount"):
            return apology("must provide symbol", 403)

        # Get username and available cash
        user = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        available_cash = user[0]["cash"]
        more_cash = request.form.get("amount")
        total_cash = float(available_cash) + float(more_cash)

        # update user's cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=total_cash, id=session["user_id"])

        flash('Congrats!')
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("cash.html")


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
