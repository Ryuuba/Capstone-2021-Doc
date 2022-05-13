from flask import Flask, Response, render_template, stream_with_context, request
from datetime import datetime
import tzlocal
import json
import time
import logging
import sys
sys.path.append('..')
import db_conn.sqlite3_conn as sqlite3_conn

logging.basicConfig(stream=sys.stdout, level=logging.INFO, format='%(asctime)s %(levelname)s %(message)s')
logger = logging.getLogger(__name__)

app = Flask(__name__)

def update_readings():
    if request.headers.getlist('X-Forwarded-For'):
        client_ip = request.headers.getlist('X-Forwarded-For')[0]
    else:
        client_ip = request.remote_addr or ''
    try:
        logger.info("Client %s connected", client_ip)
        while True:
            db_conn = sqlite3_conn.create_connection('../database/WSNReading.db')
            c722CO2_data = sqlite3_conn.get_last_data(db_conn, 'co2reading', 1, 'C722')
            c722CO2_time = datetime.fromtimestamp(
                    c722CO2_data[0][0], tzlocal.get_localzone()
                    ).strftime('%H:%M:%S')
            c722CO2_val = c722CO2_data[0][2]
            c722Temp_data = sqlite3_conn.get_last_data(db_conn, 'tempreading', 1, 'C722')
            c722Temp_time = datetime.fromtimestamp(
                    c722Temp_data[0][0], tzlocal.get_localzone()
                    ).strftime('%H:%M:%S')
            c722Temp_val = c722Temp_data[0][2]
            c722Hum_data = sqlite3_conn.get_last_data(db_conn, 'humreading', 1, 'C722')
            c722Hum_time = datetime.fromtimestamp(
                    c722Hum_data[0][0], tzlocal.get_localzone()
                    ).strftime('%H:%M:%S')
            c722Hum_val = c722Hum_data[0][2]
            print('C722CO2time: {}, C722CO2value: {}'.format(c722CO2_time, c722CO2_val))
            print('C722Temptime: {}, C722Tempvalue: {}'.format(c722Temp_time, c722Temp_val))
            print('C722Humtime: {}, C722Humvalue: {}'.format(c722Hum_time, c722Hum_val))
            db_conn.close()
            json_data = json.dumps(
                {
                    'C722CO2time': c722CO2_time, 
                    'C722CO2value': c722CO2_val,
                    'C722CO2title': 'Nivel de CO2 [ppm]',
                    'C722CO2color': 'rgb(49, 205, 49)',
                    'C722Temptime': c722Temp_time, 
                    'C722Tempvalue': c722Temp_val,
                    'C722Temptitle': 'Temperatura [°C]',
                    'C722Tempcolor': 'rgb(255, 142, 0)',
                    'C722Humtime': c722Hum_time, 
                    'C722Humvalue': c722Hum_val,
                    'C722Humtitle': 'Humedad Relativa [%]',
                    'C722Humcolor': 'rgb(72, 61, 139)'
                }
            )
            yield f'data:{json_data}\n\n'
            time.sleep(30)
    except GeneratorExit:
        logger.info("Client %s disconnected", client_ip)

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/c722plot')
def co2plot():
    return render_template('c722plot.html')


@app.route('/labredplot')
def tempplot():
    return render_template('labredplot.html')


@app.route('/chart-data')
def chart_data():
    response = Response(
        stream_with_context(update_readings()), mimetype = 'text/event-stream'
    )
    response.headers['Cache-Control'] = 'no-cache'
    response.headers['X-Accel-Buffering'] = 'no'
    return response

if __name__ == '__main__':
    print('Start server...')
    app.run(host = '0.0.0.0', threaded=True, debug=True)
