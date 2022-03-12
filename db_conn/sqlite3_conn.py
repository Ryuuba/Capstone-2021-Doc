import sqlite3
from sqlite3 import Error
from typing import Tuple

last_row = 0

def create_connection(db_path: str) -> sqlite3.Connection:
    """Creates a database connector to a SQLite database """
    try:
        db_conn = sqlite3.connect(db_path)
        print('Connection to database {} successful'.format(db_path))
    except Error as e:
        print(e)
    return db_conn

def insert_tuple(db_conn: sqlite3.Connection, reading: Tuple[str, str, int]):
    """
    Inserts a tuple <time, location, reading> into a database
    @param db_conn An initialized sqlite3 database connector
    @param reading The tuple to be inserted into the connected database
    @returns The ID of the inserted row
    """
    order = '''INSERT INTO readings(date, location, reading) VALUES(?,?,?) '''
    cursor = db_conn.cursor()
    cursor.execute(order, reading)
    db_conn.commit()
    print('Last row ID: {}'.format(cursor.lastrowid))
    global last_row
    last_row = cursor.lastrowid
    return last_row

def read_tuple(db_conn: sqlite3.Connection, row_id: int) -> Tuple[str, str, int]:
    """
    Returns a register from the database
    @param db_conn An initialized sqlite3 database connector
    @param row_id The ID of the row to be read
    @returns A tuple <date, location, reading> from the connected database
    """
    query = '''SELECT * FROM readings WHERE ROWID={} '''.format(row_id)
    cursor = db_conn.cursor()
    cursor.execute(query)
    data_tuple = (
        str(cursor.fetchall()[0]),  # UNIX timestamp
        str(cursor.fetchall()[1]),  # location
        int(cursor.fetchall()[2])   # CO2 reading
    )
    return data_tuple