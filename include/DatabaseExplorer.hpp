#pragma once
#include "DataBaseInterface.hpp"
#include <memory>

class DataBaseExplorer {
  DataBaseExplorer(std::unique_ptr<IDatabaseConnector> conn);
  DataBaseExplorer() = delete;
};
