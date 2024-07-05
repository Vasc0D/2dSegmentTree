from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import segment_tree

app = Flask(__name__, template_folder='templates')
CORS(app)

# Almacenamiento de los árboles de segmentos
segment_trees = {}

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/initialize', methods=['POST'])
def initialize_tree():
    data = request.json['data']
    tree_id = len(segment_trees)
    segment_trees[tree_id] = segment_tree.SegmentTree2D(data)
    return jsonify({"tree_id": tree_id})

@app.route('/update', methods=['POST'])
def update():
    tree_id = request.json['tree_id']
    x = request.json['x']
    y = request.json['y']
    new_val = request.json['new_val']
    segment_trees[tree_id].update(x, y, new_val)
    return jsonify({"status": "success"})

@app.route('/query', methods=['POST'])
def sum():
    tree_id = request.json.get('tree_id', 0)
    lx = int(request.json['lx'])
    rx = int(request.json['rx'])
    ly = int(request.json['ly'])
    ry = int(request.json['ry'])
    result = segment_trees[tree_id].sum(lx, rx, ly, ry)
    return jsonify({"sum": result})

@app.route('/get_data', methods=['GET'])
def get_data():
    tree_id = 0  # Suponiendo que tenemos un solo árbol para simplificar
    data = segment_trees[tree_id].data
    return jsonify(data)

@app.route('/get_tree', methods=['GET'])
def get_tree():
    tree_id = 0  # Suponiendo que tenemos un solo árbol para simplificar
    tree = segment_trees[tree_id].tree
    return jsonify(tree)

if __name__ == '__main__':
    app.run(debug=True)
