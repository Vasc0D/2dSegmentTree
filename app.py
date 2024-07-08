from flask import Flask, render_template, request, jsonify
from segment_tree import SegmentTree2D
import numpy as np

app = Flask(__name__)

# Variable global para almacenar el SegmentTree2D
segment_tree = None

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/initialize', methods=['POST'])
def initialize():
    global segment_tree
    matrix = request.json['matrix']
    segment_tree = SegmentTree2D(matrix)
    return jsonify({'message': 'Segment Tree initialized'})

@app.route('/update', methods=['POST'])
def update():
    x = request.json['x']
    y = request.json['y']
    value = request.json['value']
    segment_tree.update(x, y, value)
    return jsonify({'message': 'Value updated'})

@app.route('/sum', methods=['POST'])
def sum():
    lx = request.json['lx']
    rx = request.json['rx']
    ly = request.json['ly']
    ry = request.json['ry']
    result = segment_tree.sum(lx, rx, ly, ry)
    return jsonify({'result': result})

if __name__ == '__main__':
    app.run(debug=True)