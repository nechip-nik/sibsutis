#pragma once
#include <sqlite3.h>
#include <string>
#include <optional>

struct User { std::string login; std::string pass_hash; int rating; };

class DB {
    sqlite3* db_ = nullptr;
public:
    bool open(const std::string& path) {
        if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) return false;
        sqlite3_busy_timeout(db_, 5000);
        return true;
    }
    void init() {
        exec("CREATE TABLE IF NOT EXISTS users("
             "login TEXT PRIMARY KEY, pass_hash TEXT, rating INTEGER DEFAULT 1200)");
        exec("CREATE TABLE IF NOT EXISTS games("
             "id INTEGER PRIMARY KEY AUTOINCREMENT, white TEXT, black TEXT,"
             "result TEXT, ts DATETIME DEFAULT CURRENT_TIMESTAMP)");
    }
    bool register_user(const std::string& login, const std::string& hash) {
        return exec("INSERT INTO users(login,pass_hash) VALUES('" +
                    esc(login) + "','" + esc(hash) + "')");
    }
    std::optional<User> get_user(const std::string& login) {
        std::string sql = "SELECT login,pass_hash,rating FROM users WHERE login='" +
                          esc(login) + "'";
        sqlite3_stmt* st = nullptr;
        if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &st, nullptr) != SQLITE_OK) return {};
        std::optional<User> u;
        if (sqlite3_step(st) == SQLITE_ROW) {
            u = User{ (const char*)sqlite3_column_text(st,0),
                      (const char*)sqlite3_column_text(st,1),
                      sqlite3_column_int(st,2) };
        }
        sqlite3_finalize(st);
        return u;
    }
    void update_rating(const std::string& login, int r) {
        exec("UPDATE users SET rating=" + std::to_string(r) +
             " WHERE login='" + esc(login) + "'");
    }
    void save_game(const std::string& w, const std::string& b, const std::string& res) {
        exec("INSERT INTO games(white,black,result) VALUES('" +
             esc(w) + "','" + esc(b) + "','" + esc(res) + "')");
    }
private:
    bool exec(const std::string& sql) {
        return sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }
    static std::string esc(const std::string& s) {
        std::string o;
        for (char c : s) { if (c == '\'') o += "''"; else o += c; }
        return o;
    }
};