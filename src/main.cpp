#include "DatabaseExplorer.hpp"
#include "LoginForm.hpp"
#include "sqlite_connector.hpp"
#include <memory>
#include <string>
#include <vector>
using DatabaseRow = std::vector<std::string>;
using DatabaseResultTable = std::vector<DatabaseRow>;

int main(void) {

    LoginForm Form = {};

    Form.RUN();
    auto [host, port, database, username, password, db_type] = Form.GetConnectionParams();
    ///
    std::unique_ptr<IDatabaseConnector> conn = nullptr;
    switch (db_type) {
    case LoginForm::enum_db_type::SQLite:
        conn = std::make_unique<SQLiteConnector>();
        break;
    default:
        return 1;
    }
    if (!conn->Connect(database)) {
        return 1;
    }
    DataBaseExplorer exp = {std::move(conn)};
    exp.RUN();
    return 0;
}
