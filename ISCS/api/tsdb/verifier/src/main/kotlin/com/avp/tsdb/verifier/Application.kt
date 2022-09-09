package com.avp.tsdb.verifier

import org.slf4j.Logger
import org.slf4j.LoggerFactory

fun main(args: Array<String>) {
    val url = System.getProperty("URL", "http://127.0.0.1:8086")
    val db = System.getProperty("DB", "tsdb_iscs")
    val username = System.getProperty("username", "admin")
    val password = System.getProperty("password", "123456")
    val limit = System.getProperty("limit", "10000").toInt()
    val offset = System.getProperty("offset", "0").toInt()

    val client = TsdbClient(url, db, username, password)

    val result = client.query("SELECT * FROM bench2 LIMIT $limit OFFSET $offset")
    val rows = result?.results?.getOrNull(0)?.series?.getOrNull(0)?.rows

    val match = rows?.all { checkRow(it) }
    println(match)
}

fun checkRow(row: Map<String, *>): Boolean {
    val data = mapOf(
            "tag" to "X",
            "c0" to 0,
            "c1" to 1,
            "c2" to 2,
            "c3" to 3,
            "c4" to 4,
            "c5" to 5,
            "c6" to 6,
            "c7" to 7,
            "c8" to 8,
            "c9" to 9,
            "d0" to 0,
            "d1" to 1,
            "d2" to 2,
            "d3" to 3,
            "d4" to "一大波测试数据",
            "d5" to "二大波测试数据",
            "d6" to "三大波测试数据",
            "d7" to "四大波测试数据"
    )

    return data.all { (k, v) -> row[k] == v }
}

val Any.logger: Logger
    get () = LoggerFactory.getLogger(this::class.java)