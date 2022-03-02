#include <gtk/gtk.h>
#include <stdio.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


GtkWidget *windows; // Variable pour la fenetre
GtkWidget *windows_settings; // Variable pour la fenetre
GtkWidget *windows_game;
GtkWidget *windows_result;
GtkWidget *windows_stack0;
GtkLabel *label_firstname;

GtkLabel *label_name;
GtkLabel *label_course0;
GtkLabel *label_course1;
GtkLabel *label_course2;
GtkLabel *label_course3;
GtkLabel *label_course4;
GtkLabel *label_course5;
GtkLabel *label_course6;
GtkLabel *label_course7;
GtkLabel *label_course8;
GtkLabel *label_course9;
GtkLabel *label_course10;
GtkLabel *label_course11;
GtkLabel *label_course12;
GtkLabel *label_course13;
GtkLabel *label_course14;
GtkLabel *label_course15;
GtkLabel *label_course16;
GtkLabel *label_course17;
GtkLabel *label_course18;
GtkLabel *label_course19;
GtkLabel *label_course20;
GtkLabel *label_course21;
GtkLabel *label_course22;
GtkLabel *label_course23;
GtkLabel *label_course24;
GtkLabel *label_course25;
GtkLabel *label_course26;
GtkLabel *label_course27;
GtkLabel *label_course28;
GtkLabel *label_course29;
GtkLabel *label_course30;
GtkLabel *label_course31;
GtkLabel *label_course32;
GtkLabel *label_course33;
GtkLabel *label_course34;
GtkLabel *label_course35;
GtkLabel *label_course36;
GtkLabel *label_course37;
GtkLabel *label_course38;
GtkLabel *label_course39;
GtkLabel *label_course40;
GtkLabel *label_course41;

GtkLabel *label_mark0;
GtkLabel *label_mark1;
GtkLabel *label_mark2;
GtkLabel *label_mark3;
GtkLabel *label_mark4;
GtkLabel *label_mark5;
GtkLabel *label_mark6;
GtkLabel *label_mark7;
GtkLabel *label_mark8;
GtkLabel *label_mark9;
GtkLabel *label_mark10;
GtkLabel *label_mark11;
GtkLabel *label_mark12;
GtkLabel *label_mark13;
GtkLabel *label_mark14;
GtkLabel *label_mark15;
GtkLabel *label_mark16;
GtkLabel *label_mark17;
GtkLabel *label_mark18;
GtkLabel *label_mark19;
GtkLabel *label_mark20;
GtkLabel *label_mark21;
GtkLabel *label_mark22;
GtkLabel *label_mark23;
GtkLabel *label_mark24;
GtkLabel *label_mark25;
GtkLabel *label_mark26;
GtkLabel *label_mark27;
GtkLabel *label_mark28;
GtkLabel *label_mark29;
GtkLabel *label_mark30;
GtkLabel *label_mark31;
GtkLabel *label_mark32;
GtkLabel *label_mark33;
GtkLabel *label_mark34;
GtkLabel *label_mark35;
GtkLabel *label_mark36;
GtkLabel *label_mark37;
GtkLabel *label_mark38;
GtkLabel *label_mark39;
GtkLabel *label_mark40;
GtkLabel *label_mark41;




struct json_object *parsed_json;
struct json_object *firstname;
struct json_object *name;
struct json_object *photo;
struct json_object *course;
struct json_object *grades;
struct json_object *trimester;
struct json_object *teacher_first_name;
struct json_object *teacher_last_name;
struct json_object *value;
struct json_object *value1;
struct json_object *value_in_1;

char *json_received;

struct curl_fetch_st {
    char *payload;
    size_t size;
};

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);
    if (p->payload == NULL) {
        fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
        free(p->payload);
        return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);
    p->size += realsize;
    p->payload[p->size] = 0;
    return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
    CURLcode rcode;                   /* curl result code */

    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));
    if (fetch->payload == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        return CURLE_FAILED_INIT;
    }

    fetch->size = 0;
    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);
    return rcode;
}

struct curl_fetch_st curl_fetch;                        /* curl fetch struct */

char *curl_receive_reply_json_to_buffer(char *url, char *userpwd) {
    CURL *ch;                                               /* curl handle */
    CURLcode rcode;                                         /* curl result code */
    struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
    struct curl_slist *headers = NULL;                      /* http headers to send with request */

    /*TODO: when to free above malloced url*/

    /* init curl handle */
    if ((ch = curl_easy_init()) == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        /* return error */
        return NULL;
    }

    /* set content type */
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(ch, CURLOPT_HTTPGET, 1L);

    if (userpwd != NULL) {
        curl_easy_setopt(ch, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0);
    }

    /* fetch page and capture return code */
    rcode = curl_fetch_url(ch, url, cf);

    /* cleanup curl handle */
    curl_easy_cleanup(ch);

    /* free headers */
    curl_slist_free_all(headers);
    /* check return code */
    if (rcode != CURLE_OK || cf->size < 1) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
                url, curl_easy_strerror(rcode));
        /* return error */
        return NULL; // TODO: how to handle "curl said: Timeout was reached"
    }

    /* check payload */
    if (cf->payload != NULL) {
        /* print result */
        return cf->payload;
    }

    /* error */
    fprintf(stderr, "ERROR: Failed to populate payload");
    /* free payload */
    free(cf->payload);
    /* return */
    return NULL;
}

char *curl_download_json_to_buffer(char *remote_url) {
    char *reply_json_received;
    int wait_count = 0;

    while(wait_count < 10) {
        reply_json_received = curl_receive_reply_json_to_buffer(remote_url, NULL);
        if (reply_json_received == NULL) {
            wait_count++;
            printf("failed for %d th time, retrying after 3 sec...\n", wait_count);
            sleep(5); //sleep for 5 sec and retry
            continue;
        } else
            break;
    }


    json_received = reply_json_received;

    return reply_json_received;
}

int main(int argc, char *argv[]) {
    // Curl
    curl_download_json_to_buffer("http://146.59.154.231/api_myges/index.php");

    // Parser JSON



    parsed_json = json_tokener_parse(json_received);

    value = json_object_array_get_idx(parsed_json, 0);
    value1 = json_object_array_get_idx(parsed_json, 1);

    // printf("\nICI: %s\n", json_object_get_string(value_in_1));

    json_object_object_get_ex(value, "firstname", &firstname);
    json_object_object_get_ex(value, "name", &name);
    json_object_object_get_ex(value, "photo", &photo);


    printf("\nPrénom: %s\n", json_object_get_string(firstname));
    printf("Nom: %s\n", json_object_get_string(name));
    printf("Photo: %s\n", json_object_get_string(photo));



    for (int i=0; i<26; i++){
        //json_object_object_get_ex(value_in_1, "trimester", &trimester);
        //if (strcmp(json_object_get_string(trimester), "21")){
            value_in_1 = json_object_array_get_idx(value1, i);

            json_object_object_get_ex(value_in_1, "course", &course);
            json_object_object_get_ex(value_in_1, "grades", &grades);
            json_object_object_get_ex(value_in_1, "teacher_first_name", &teacher_first_name);
            json_object_object_get_ex(value_in_1, "teacher_last_name", &teacher_last_name);

            printf("\nMatiere: %s\n", json_object_get_string(course));
            printf("Notes: %s\n", json_object_get_string(grades));
            printf("Prenom du professeur: %s\n", json_object_get_string(teacher_first_name));
            printf("Nom du professeur: %s\n", json_object_get_string(teacher_last_name));
        //}
    }


    // Gtk/Glade
    GtkBuilder *builder; // Pour le build
    gtk_init(&argc, &argv); // Lancer gtk

    builder = gtk_builder_new(); // Crée le build
    gtk_builder_add_from_file(builder, "start.glade", NULL); // Charger le fichier

    windows = GTK_WIDGET(gtk_builder_get_object(builder, "start")); // Charger la fenetre

    // Nécessaire pour un code qui utilise gtk
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(windows); // Afficher notre fenetre
    gtk_main(); // Lunch

    return 0;
}
// Pour quitter l'app
void exit_app(){
    printf("Quiiter \n");
    gtk_main_quit(); // commande pour quitter
}

// Reaction du button
void button_settings(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "settings.glade", NULL); // Charger le fichier

    windows_settings = GTK_WIDGET(gtk_builder_get_object(builder, "windows_settings"));
    gtk_window_set_default_size(GTK_WINDOW(windows_settings), 800, 400);
    g_signal_connect(windows_settings, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_settings);
}

void destroy_settings(){
    gtk_widget_destroy(windows_settings);
}

void button_game(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "game.glade", NULL); // Charger le fichier

    windows_game = GTK_WIDGET(gtk_builder_get_object(builder, "windows_game"));
    gtk_window_set_default_size(GTK_WINDOW(windows_game), 1920, 1080);

    label_firstname = GTK_LABEL(gtk_builder_get_object(builder, "surname_student"));
    gtk_label_set_text(label_firstname, json_object_get_string(firstname));

    label_name = GTK_LABEL(gtk_builder_get_object(builder, "name_student"));
    gtk_label_set_text(label_name, json_object_get_string(name));

    windows_stack0 = GTK_WIDGET(gtk_builder_get_object(builder, "windows_stack0"));
    gtk_window_set_title(GTK_WINDOW(windows_stack0), json_object_get_string(name));
    

    for (int y=0; y<13; y++){
        value_in_1 = json_object_array_get_idx(value1, y);

        json_object_object_get_ex(value_in_1, "course", &course);
        json_object_object_get_ex(value_in_1, "grades", &grades);
        json_object_object_get_ex(value_in_1, "teacher_first_name", &teacher_first_name);
        json_object_object_get_ex(value_in_1, "teacher_last_name", &teacher_last_name);

        switch (y) {
            case 0:
                label_course0 = GTK_LABEL(gtk_builder_get_object(builder, "label_course0"));
                gtk_label_set_text(label_course0, json_object_get_string(course));
                label_course14 = GTK_LABEL(gtk_builder_get_object(builder, "label_course14"));
                gtk_label_set_text(label_course14, json_object_get_string(course));
                label_course28 = GTK_LABEL(gtk_builder_get_object(builder, "label_course28"));
                gtk_label_set_text(label_course28, json_object_get_string(course));

                label_mark0 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark0"));
                gtk_label_set_text(label_mark0, json_object_get_string(grades));
                label_mark14 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark14"));
                gtk_label_set_text(label_mark14, "[ 14 ]");
                label_mark28 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark28"));
                gtk_label_set_text(label_mark28, "[ 12 ]");

            case 1:
                label_course1 = GTK_LABEL(gtk_builder_get_object(builder, "label_course1"));
                gtk_label_set_text(label_course1, json_object_get_string(course));
                label_course15 = GTK_LABEL(gtk_builder_get_object(builder, "label_course15"));
                gtk_label_set_text(label_course15, json_object_get_string(course));
                label_course29 = GTK_LABEL(gtk_builder_get_object(builder, "label_course29"));
                gtk_label_set_text(label_course29, json_object_get_string(course));

                label_mark1 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark1"));
                gtk_label_set_text(label_mark1, json_object_get_string(grades));
                label_mark15 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark15"));
                gtk_label_set_text(label_mark15, "[ 12, 15.5 ]");
                label_mark29 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark29"));
                gtk_label_set_text(label_mark29, "[ 14, 11 ]");

            case 2:
                label_course2 = GTK_LABEL(gtk_builder_get_object(builder, "label_course2"));
                gtk_label_set_text(label_course2, json_object_get_string(course));
                label_course16 = GTK_LABEL(gtk_builder_get_object(builder, "label_course16"));
                gtk_label_set_text(label_course16, json_object_get_string(course));
                label_course30 = GTK_LABEL(gtk_builder_get_object(builder, "label_course30"));
                gtk_label_set_text(label_course30, json_object_get_string(course));

                label_mark2 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark2"));
                gtk_label_set_text(label_mark2, json_object_get_string(grades));
                label_mark16 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark16"));
                gtk_label_set_text(label_mark16, "[ ]");
                label_mark30 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark30"));
                gtk_label_set_text(label_mark30, "[ ]");

            case 3:
                label_course3 = GTK_LABEL(gtk_builder_get_object(builder, "label_course3"));
                gtk_label_set_text(label_course3, json_object_get_string(course));
                label_course17 = GTK_LABEL(gtk_builder_get_object(builder, "label_course17"));
                gtk_label_set_text(label_course17, json_object_get_string(course));
                label_course31 = GTK_LABEL(gtk_builder_get_object(builder, "label_course31"));
                gtk_label_set_text(label_course31, json_object_get_string(course));

                label_mark3 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark3"));
                gtk_label_set_text(label_mark3, json_object_get_string(grades));
                label_mark17 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark17"));
                gtk_label_set_text(label_mark17, "[ 12, 17 ]");
                label_mark31 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark31"));
                gtk_label_set_text(label_mark31, "[ 8, 13 ]");

            case 4:
                label_course4 = GTK_LABEL(gtk_builder_get_object(builder, "label_course4"));
                gtk_label_set_text(label_course4, json_object_get_string(course));
                label_course18 = GTK_LABEL(gtk_builder_get_object(builder, "label_course18"));
                gtk_label_set_text(label_course18, json_object_get_string(course));
                label_course32 = GTK_LABEL(gtk_builder_get_object(builder, "label_course32"));
                gtk_label_set_text(label_course32, json_object_get_string(course));

                label_mark4 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark4"));
                gtk_label_set_text(label_mark4, json_object_get_string(grades));
                label_mark18 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark18"));
                gtk_label_set_text(label_mark18, "[ 13 ]");
                label_mark32 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark32"));
                gtk_label_set_text(label_mark32, "[ 14 ]");

            case 5:
                label_course5 = GTK_LABEL(gtk_builder_get_object(builder, "label_course5"));
                gtk_label_set_text(label_course5, json_object_get_string(course));
                label_course19= GTK_LABEL(gtk_builder_get_object(builder, "label_course19"));
                gtk_label_set_text(label_course19, json_object_get_string(course));
                label_course33 = GTK_LABEL(gtk_builder_get_object(builder, "label_course33"));
                gtk_label_set_text(label_course33, json_object_get_string(course));

                label_mark5 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark5"));
                gtk_label_set_text(label_mark5, json_object_get_string(grades));
                label_mark19 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark19"));
                gtk_label_set_text(label_mark19, "[ 16 ]");
                label_mark33 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark33"));
                gtk_label_set_text(label_mark33, "[ 12 ]");

            case 6:
                label_course6 = GTK_LABEL(gtk_builder_get_object(builder, "label_course6"));
                gtk_label_set_text(label_course6, json_object_get_string(course));
                label_course20 = GTK_LABEL(gtk_builder_get_object(builder, "label_course20"));
                gtk_label_set_text(label_course20, json_object_get_string(course));
                label_course34 = GTK_LABEL(gtk_builder_get_object(builder, "label_course34"));
                gtk_label_set_text(label_course34, json_object_get_string(course));

                label_mark6 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark6"));
                gtk_label_set_text(label_mark6, json_object_get_string(grades));
                label_mark20 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark20"));
                gtk_label_set_text(label_mark20, "[ 16, 10 ]");
                label_mark34 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark34"));
                gtk_label_set_text(label_mark34, "[ 7, 13 ]");

            case 7:
                label_course7 = GTK_LABEL(gtk_builder_get_object(builder, "label_course7"));
                gtk_label_set_text(label_course7, json_object_get_string(course));
                label_course21 = GTK_LABEL(gtk_builder_get_object(builder, "label_course21"));
                gtk_label_set_text(label_course21, json_object_get_string(course));
                label_course35 = GTK_LABEL(gtk_builder_get_object(builder, "label_course35"));
                gtk_label_set_text(label_course35, json_object_get_string(course));

                label_mark7 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark7"));
                gtk_label_set_text(label_mark7, json_object_get_string(grades));
                label_mark21 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark21"));
                gtk_label_set_text(label_mark21, "[ 18 ]");
                label_mark35 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark35"));
                gtk_label_set_text(label_mark35, "[ 18 ]");

            case 8:
                label_course8 = GTK_LABEL(gtk_builder_get_object(builder, "label_course8"));
                gtk_label_set_text(label_course8, json_object_get_string(course));
                label_course22 = GTK_LABEL(gtk_builder_get_object(builder, "label_course22"));
                gtk_label_set_text(label_course22, json_object_get_string(course));
                label_course36 = GTK_LABEL(gtk_builder_get_object(builder, "label_course36"));
                gtk_label_set_text(label_course36, json_object_get_string(course));

                label_mark8 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark8"));
                gtk_label_set_text(label_mark8, json_object_get_string(grades));
                label_mark22 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark22"));
                gtk_label_set_text(label_mark22, "[ ]");
                label_mark36 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark36"));
                gtk_label_set_text(label_mark36, "[ ]");

            case 9:
                label_course9 = GTK_LABEL(gtk_builder_get_object(builder, "label_course9"));
                gtk_label_set_text(label_course9, json_object_get_string(course));
                label_course23 = GTK_LABEL(gtk_builder_get_object(builder, "label_course23"));
                gtk_label_set_text(label_course23, json_object_get_string(course));
                label_course37 = GTK_LABEL(gtk_builder_get_object(builder, "label_course37"));
                gtk_label_set_text(label_course37, json_object_get_string(course));

                label_mark9 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark9"));
                gtk_label_set_text(label_mark9, json_object_get_string(grades));
                label_mark23 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark23"));
                gtk_label_set_text(label_mark23, "[ 5 ]");
                label_mark37 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark37"));
                gtk_label_set_text(label_mark37, "[ 2 ]");

            case 10:
                label_course10 = GTK_LABEL(gtk_builder_get_object(builder, "label_course10"));
                gtk_label_set_text(label_course10, json_object_get_string(course));
                label_course24 = GTK_LABEL(gtk_builder_get_object(builder, "label_course24"));
                gtk_label_set_text(label_course24, json_object_get_string(course));
                label_course38 = GTK_LABEL(gtk_builder_get_object(builder, "label_course38"));
                gtk_label_set_text(label_course38, json_object_get_string(course));

                label_mark10 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark10"));
                gtk_label_set_text(label_mark10, json_object_get_string(grades));
                label_mark24 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark24"));
                gtk_label_set_text(label_mark24, "[ ]");
                label_mark38 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark38"));
                gtk_label_set_text(label_mark38, "[ ]");

            case 11:
                label_course11 = GTK_LABEL(gtk_builder_get_object(builder, "label_course11"));
                gtk_label_set_text(label_course11, json_object_get_string(course));
                label_course25 = GTK_LABEL(gtk_builder_get_object(builder, "label_course25"));
                gtk_label_set_text(label_course25, json_object_get_string(course));
                label_course39 = GTK_LABEL(gtk_builder_get_object(builder, "label_course39"));
                gtk_label_set_text(label_course39, json_object_get_string(course));

                label_mark11 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark11"));
                gtk_label_set_text(label_mark11, json_object_get_string(grades));
                label_mark25 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark25"));
                gtk_label_set_text(label_mark25, "[ ]");
                label_mark39 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark39"));
                gtk_label_set_text(label_mark39, "[ ]");

            case 12:
                label_course12 = GTK_LABEL(gtk_builder_get_object(builder, "label_course12"));
                gtk_label_set_text(label_course12, json_object_get_string(course));
                label_course26 = GTK_LABEL(gtk_builder_get_object(builder, "label_course26"));
                gtk_label_set_text(label_course26, json_object_get_string(course));
                label_course40 = GTK_LABEL(gtk_builder_get_object(builder, "label_course40"));
                gtk_label_set_text(label_course40, json_object_get_string(course));

                label_mark12 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark12"));
                gtk_label_set_text(label_mark12, json_object_get_string(grades));
                label_mark26 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark26"));
                gtk_label_set_text(label_mark26, "[ ]");
                label_mark40 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark40"));
                gtk_label_set_text(label_mark40, "[ ]");

            case 13:
                label_course13 = GTK_LABEL(gtk_builder_get_object(builder, "label_course13"));
                gtk_label_set_text(label_course13, json_object_get_string(course));
                label_course27 = GTK_LABEL(gtk_builder_get_object(builder, "label_course27"));
                gtk_label_set_text(label_course27, json_object_get_string(course));
                label_course41 = GTK_LABEL(gtk_builder_get_object(builder, "label_course41"));
                gtk_label_set_text(label_course41, json_object_get_string(course));

                label_mark13 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark13"));
                gtk_label_set_text(label_mark13, json_object_get_string(grades));
                label_mark27 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark27"));
                gtk_label_set_text(label_mark27, "[ ]");
                label_mark41 = GTK_LABEL(gtk_builder_get_object(builder, "label_mark41"));
                gtk_label_set_text(label_mark41, "[ ]");
        }
    }

    g_signal_connect(windows_game, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_game);
}
GtkWidget *windows_result0;
GtkWidget *windows_result1;
GtkWidget *windows_result2;

void button_result(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "game.glade", NULL); // Charger le fichier

    windows_game = GTK_WIDGET(gtk_builder_get_object(builder, "windows_game"));
    gtk_window_set_default_size(GTK_WINDOW(windows_game), 1920, 1080);

    windows_result0 = GTK_WIDGET(gtk_builder_get_object(builder, "windows_result0"));

    //gtk_builder_connect_signals(builder, NULL);
    //gtk_widget_show_all(windows_game);
    //gtk_widget_destroy(windows_result0);
}

void button_end(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "result.glade", NULL); // Charger le fichier

    windows_result = GTK_WIDGET(gtk_builder_get_object(builder, "windows_result"));
    gtk_window_set_default_size(GTK_WINDOW(windows_result), 1920, 1080);
    g_signal_connect(windows_result, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_result);
}



