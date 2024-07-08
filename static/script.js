let sumMode = false;
let editMode = false;
let sumStart = null;
let sumEnd = null;
let selectedCell = null;

function initializeMatrix() {
    const rows = parseInt($('#rows').val());
    const cols = parseInt($('#cols').val());
    if (rows < 1 || rows > 10 || cols < 1 || cols > 10) {
        $('#result').text('Please enter dimensions between 1 and 10');
        return;
    }

    let matrixHtml = '<table>';
    for (let i = 0; i < rows; i++) {
        matrixHtml += '<tr>';
        for (let j = 0; j < cols; j++) {
            matrixHtml += `<td><input type="text" class="matrix-cell" data-row="${i}" data-col="${j}" value="0" readonly></td>`;
        }
        matrixHtml += '</tr>';
    }
    matrixHtml += '</table>';
    $('#matrix').html(matrixHtml);

    $('.matrix-cell').on('click', function() {
        if (editMode) {
            selectForEdit($(this));
        } else if (sumMode) {
            selectForSum($(this));
        }
    });

    $('#result').text('Matrix initialized');
    $('#saveMatrix').prop('disabled', false);
    $('#enableEdit').prop('disabled', true);
    $('#toggleSumMode').prop('disabled', true);
    $('#updateValue').prop('disabled', true);
}

function saveMatrix() {
    const rows = $('.matrix-cell').first().closest('table').find('tr').length;
    const cols = $('.matrix-cell').first().closest('tr').find('td').length;
    let matrix = [];

    for (let i = 0; i < rows; i++) {
        let row = [];
        for (let j = 0; j < cols; j++) {
            row.push(parseInt($(`input[data-row="${i}"][data-col="${j}"]`).val()) || 0);
        }
        matrix.push(row);
    }

    $.ajax({
        url: '/initialize',
        method: 'POST',
        contentType: 'application/json',
        data: JSON.stringify({ matrix: matrix }),
        success: function(response) {
            $('#result').text('Matrix saved');
            $('.matrix-cell').prop('readonly', true);
            $('#saveMatrix').prop('disabled', true);
            $('#enableEdit').prop('disabled', false);
            $('#toggleSumMode').prop('disabled', false);
        }
    });
}

function enableEdit() {
    editMode = true;
    sumMode = false;
    $('#result').text('Edit mode enabled. Select a cell to edit.');
    $('#enableEdit').prop('disabled', true);
    $('#toggleSumMode').prop('disabled', true);
    $('#updateValue').prop('disabled', false);
}

function selectForEdit(cell) {
    if (selectedCell) {
        selectedCell.prop('readonly', true);
    }
    selectedCell = cell;
    cell.prop('readonly', false);
    $('#result').text(`Editing cell (${cell.data('row')}, ${cell.data('col')})`);
}

function updateValue() {
    if (!selectedCell) {
        $('#result').text('Please select a cell to edit first');
        return;
    }

    const row = parseInt(selectedCell.data('row'));
    const col = parseInt(selectedCell.data('col'));
    const value = parseInt(selectedCell.val()) || 0;

    $.ajax({
        url: '/update',
        method: 'POST',
        contentType: 'application/json',
        data: JSON.stringify({ x: row, y: col, value: value }),
        success: function(response) {
            $('#result').text(`Updated (${row}, ${col}) to ${value}`);
            selectedCell.prop('readonly', true);
            selectedCell = null;
            editMode = false;
            $('#enableEdit').prop('disabled', false);
            $('#toggleSumMode').prop('disabled', false);
            $('#updateValue').prop('disabled', true);
        }
    });
}

function toggleSumMode() {
    sumMode = !sumMode;
    editMode = false;
    sumStart = null;
    sumEnd = null;
    $('.matrix-cell').removeClass('selected');
    $('#result').text(sumMode ? 'Sum mode ON. Select two cells for range.' : 'Sum mode OFF');
}

function selectForSum(cell) {
    if (!sumStart) {
        sumStart = { row: parseInt(cell.data('row')), col: parseInt(cell.data('col')) };
        cell.addClass('selected');
        $('#result').text('First cell selected. Select second cell for range.');
    } else if (!sumEnd) {
        sumEnd = { row: parseInt(cell.data('row')), col: parseInt(cell.data('col')) };
        cell.addClass('selected');
        calculateSum();
    } else {
        $('.matrix-cell').removeClass('selected');
        sumStart = { row: parseInt(cell.data('row')), col: parseInt(cell.data('col')) };
        sumEnd = null;
        cell.addClass('selected');
        $('#result').text('First cell selected. Select second cell for range.');
    }
}

function calculateSum() {
    if (!sumStart || !sumEnd) return;

    const lx = Math.min(sumStart.row, sumEnd.row);
    const rx = Math.max(sumStart.row, sumEnd.row);
    const ly = Math.min(sumStart.col, sumEnd.col);
    const ry = Math.max(sumStart.col, sumEnd.col);

    $.ajax({
        url: '/sum',
        method: 'POST',
        contentType: 'application/json',
        data: JSON.stringify({ lx: lx, rx: rx, ly: ly, ry: ry }),
        success: function(response) {
            $('#result').text(`Sum from (${lx}, ${ly}) to (${rx}, ${ry}): ${response.result}`);
            sumMode = false;
            $('.matrix-cell').removeClass('selected');
        }
    });
}

$(document).ready(function() {
    initializeMatrix();
});