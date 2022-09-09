package com.avp.tsdb.verifier

import okhttp3.OkHttpClient
import okhttp3.Request
import java.net.URLEncoder

class TsdbClient(url: String, db: String, username: String, password: String) {
    private val baseUrl = "$url/query?db=$db&u=$username&p=$password"
    private val client = OkHttpClient()

    fun query(sql: String): TsdbResult? {
        val q = URLEncoder.encode(sql, Charsets.UTF_8.name())
        val request = Request.Builder().url("$baseUrl&q=$q").build()
        val response = client.newCall(request).execute().takeIf { it.isSuccessful }?.body()?.string()
        return KSON.decode(response)
    }
}

data class TsdbResult(val results: List<QueryResult>)

data class QueryResult(val series: List<TsdbSeries>)

data class TsdbSeries(val name: String, val columns: List<String>, val values: List<List<*>>) {
    val rows: List<Map<String, *>> by lazy {
        values.map { value ->
            columns.mapIndexed { index, s -> s to value[index] }.toMap()
        }
    }
}