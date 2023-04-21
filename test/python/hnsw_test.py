import duckdb

def test_hnsw():
    conn = duckdb.connect('');
    conn.execute("SELECT hnsw('Sam') as value;");
    res = conn.fetchall()
    assert(res[0][0] == "Hnsw Sam 🐥");