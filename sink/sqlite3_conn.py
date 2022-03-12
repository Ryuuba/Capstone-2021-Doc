import sqlite3
from sqlite3 import Error
from typing import Tuple

def create_connection(db_path: str) -> sqlite3.Connection:
    """Creates a database connector to a SQLite database """
    try:
        db_conn = sqlite3.connect(db_path)
        print('Connection to database {} successful'.format(db_path))
    except Error as e:
        print(e)
    return db_conn

def insert_tuple(db_conn: sqlite3.Connection, reading: Tuple[str, str, int]):
    """Inserts a tuple <time, location, reading> into a database"""
    query = '''INSERT INTO readings(date, location, reading) VALUES(?,?,?) '''
    cursor = db_conn.cursor()
    cursor.execute(query, reading)
    db_conn.commit()
    print('Last row ID: {}'.format(cursor.lastrowid))
    return cursor.lastrowid
