from flask import Flask, url_for, redirect, render_template, request
import os
import subprocess
app = Flask(__name__)

@app.route('/')
def busproject():
    return render_template('content/index.html')

@app.route('/bus_number')
def bus_number():
    return render_template('content/bus_number.html')

@app.route('/post', methods=['GET','POST'])
def post():
    if request.method == 'POST':
        value = request.form['userBusNumber']
        value = str(value)
        print(value)
        os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
        subprocess.call(["save-to-disk.exe","bnum",value])
        # a = subprocess.call(["Project136.exe", value])
    # return render_template('content/post.html')
    return redirect(url_for('busproject'))

@app.route('/bell')
def bell():
    # os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
    # os.startfile("save-to-disk.exe")
    os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
    subprocess.call(["save-to-disk.exe","bell"])
    # return render_template('content/bell.html')
    return redirect(url_for('busproject'))

@app.route('/seat')
def seat():
    os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
    subprocess.call(["save-to-disk.exe","seat"])
    # return render_template('content/seat.html')
    return redirect(url_for('busproject'))

@app.route('/entrance')
def entrance():
    os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
    subprocess.call(["save-to-disk.exe","entrance"])
    # return render_template('content/entrance.html')
    return redirect(url_for('busproject'))

# @app.route('/exit')
# def exit():
#     os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
#     subprocess.call(["save-to-disk.exe","exit"])
#     return render_template('content/exit.html')

@app.route('/terminal')
def terminal():
    os.chdir('C:\\Program Files (x86)\\Intel RealSense SDK 2.0\\samples\\x64\\Debug')
    subprocess.call(["save-to-disk.exe","terminal"])
    # return render_template('content/terminal.html')
    return redirect(url_for('busproject'))



if __name__ == '__main__':
    app.debug = True
    app.run()