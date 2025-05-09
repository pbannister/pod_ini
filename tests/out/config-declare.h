#pragma once
// Generated date: Mon May  5 07:56:10 2025
// Do not edit this file.

extern struct configuration_pod_t {
    ~configuration_pod_t();
    struct context_o;
    struct context_o* p_context;
    bool pod_load(const char*);
    bool pod_save(const char*);
    struct configuration_pod_foo_t {
        const char* v_bar;
        const char* v_zot;
    } s_foo;
    struct configuration_pod_panic_t {
        const char* v_when;
    } s_panic;
    struct configuration_pod_us_bannister_reader_t {
        const char* v_application;
        const char* v_service;
        const char* v_version;
    } s_us_bannister_reader;
    struct configuration_pod_us_bannister_reader_tinyrss_t {
        const char* v_application;
        const char* v_service;
        const char* v_version;
    } s_us_bannister_reader_tinyrss;
    struct configuration_pod_user_t {
        const char* v_email;
        const char* v_location;
        const char* v_name;
        const char* v_site;
        const char* v_timezone;
    } s_user;
} configuration_pod;
