package com.avp.tsdb.verifier

import com.fasterxml.jackson.annotation.JsonInclude
import com.fasterxml.jackson.databind.DeserializationFeature
import com.fasterxml.jackson.module.kotlin.jacksonObjectMapper
import com.fasterxml.jackson.module.kotlin.readValue

object KSON {
    private val encodeWriter = jacksonObjectMapper().writer()
    private val decodeMapper = jacksonObjectMapper().apply {
        configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false)
    }
    private val encodeNonNullWriter = jacksonObjectMapper().apply {
        setSerializationInclusion(JsonInclude.Include.NON_NULL)
    }.writer()

    fun encode(obj: Any?): String = encodeWriter.writeValueAsString(obj)

    fun encodeNonNull(obj: Any?): String = encodeNonNullWriter.writeValueAsString(obj)

    internal inline fun <reified T : Any> decode(json: String?) = try {
        json?.let { decodeMapper.readValue<T>(it) }
    } catch (e: Throwable) {
        null
    }
}
