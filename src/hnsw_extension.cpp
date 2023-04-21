#define DUCKDB_EXTENSION_MAIN

#include "hnsw_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "hnswlib.h"

#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

namespace duckdb {

inline void HnswScalarFun(DataChunk &args, ExpressionState &state,
                          Vector &result) {
  auto &name_vector = args.data[0];
  UnaryExecutor::Execute<string_t, string_t>(
      name_vector, result, args.size(), [&](string_t name) {
        return StringVector::AddString(result,
                                       "Hnsw " + name.GetString() + " 🐥");
      });
}

static void LoadInternal(DatabaseInstance &instance) {
  Connection con(instance);
  con.BeginTransaction();

  auto &catalog = Catalog::GetSystemCatalog(*con.context);

  CreateScalarFunctionInfo hnsw_fun_info(ScalarFunction(
      "hnsw", {LogicalType::VARCHAR}, LogicalType::VARCHAR, HnswScalarFun));
  hnsw_fun_info.on_conflict = OnCreateConflict::ALTER_ON_CONFLICT;
  catalog.CreateFunction(*con.context, &hnsw_fun_info);
  con.Commit();
}

void HnswExtension::Load(DuckDB &db) { LoadInternal(*db.instance); }

std::string HnswExtension::Name() { return "hnsw"; }

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void hnsw_init(duckdb::DatabaseInstance &db) {
  LoadInternal(db);
}

DUCKDB_EXTENSION_API const char *hnsw_version() {
  return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
