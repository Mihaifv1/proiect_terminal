#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "liste.h"
#include "stack.h"

/*
 - exit codes:
     0 - programul a rulat ok
     1 - input/output errror
     2 - input file nu s-a deschis
     3 - output file nu s-a deschis
*/

#define DEBUG 0
#define AFIS  0

int esteNumar(const char *s) 
{
    const char num[] = "0123456789";
    for (size_t i = 0; i < strlen(s); i++) 
    {
        if (strchr(num, s[i]) == NULL)
            return 0;
    }
    return 1;
}

int esteFisier(const char *s) 
{
    return (strstr(s, "--file") || strstr(s, "-f") ||
            strstr(s, "--out")  || strstr(s, "-o"));
}

void func(int argc, char **argv, ...) 
{
    va_list parametrii;
    va_start(parametrii, argv);

    for (int index_param = 0; index_param < 3; index_param++) 
    {
        Lista **arg_curent = va_arg(parametrii, Lista **);
        int still_args = 1;

        for (int i = 2; i < argc; i++) 
        {
            if (argv[i][0] == '-')
                still_args = 0;


            if (argv[i][0] != '-' && index_param == 0 && still_args) 
            {
                Argument arg;
                arg.tip = 'a';
                arg.valoare = malloc(strlen(argv[i]) + 1);
                strcpy(arg.valoare, argv[i]);
                arg.val_opt = NULL;
                arg.format = 'N';
                adaugare_element_final(arg_curent, arg);
            }

            else if (argv[i][0] == '-' && index_param == 1 &&
                     (strchr(argv[i], '=') ||
                      (i+1 < argc && esteNumar(argv[i+1])) ||
                       esteFisier(argv[i]))) 
            {
                Argument arg;
                arg.tip = 'o';
                arg.format = (argv[i][1] == '-') ? 'l' : 's';
                if (!strchr(argv[i], '=')) 
                {
                    char *s = strdup(argv[i]);
                    arg.valoare = strdup(s);
                    i++;
                    arg.val_opt = strdup(argv[i]);
                    free(s);
                } else 
                {
                    char *s = strdup(argv[i]);
                    char *tok = strtok(s, "=");
                    arg.valoare = strdup(tok);
                    tok = strtok(NULL, "=");
                    arg.val_opt = strdup(tok);
                    free(s);
                }
                adaugare_element_final(arg_curent, arg);
            }

            else if (argv[i][0] == '-' && index_param == 2 && !esteFisier(argv[i])) 
            {
                if (argv[i][1] != '-') 
                {
                    int nr_flags = strlen(argv[i]+1);
                    for (int j = 1; j <= nr_flags; j++) 
                    {
                        Argument arg;
                        arg.tip      = 'f';
                        char tmp[3]  = {'-', argv[i][j], '\0'};
                        arg.valoare  = strdup(tmp);
                        arg.val_opt  = strdup("1");
                        arg.format   = 's';
                        adaugare_element_final(arg_curent, arg);
                    }
                } else 
                {
                    Argument arg;
                    arg.tip     = 'f';
                    arg.valoare = strdup(argv[i]);
                    arg.val_opt = strdup("1");
                    arg.format  = 'l';
                    adaugare_element_final(arg_curent, arg);
                }
            }
        }
    }

    va_end(parametrii);
}

char gasire_param(const char *s) 
{
    char param = '-';
    size_t len = strlen(s);
    for (size_t i = 0; i < len; i++) 
    {
        if (i == 0 && s[i] == 'a')
            param = 'a';
        else if (i > 0 && s[i] == 'o' && s[i-1]==' ' &&
                 (s[i+1]=='\0' || s[i+1]=='\n'))
            param = 'o';
        else if (i > 0 && s[i] == 'f' && s[i-1]==' ' &&
                 (s[i+1]=='\0' || s[i+1]=='\n'))
            param = 'f';
    }
    return param;
}

void getDirs(char crt_dir[], Stack **S, int level) 
{
    DIR *dir = opendir(crt_dir);
    if (!dir) 
    {
        printf("Nu s-a putut deschide directorul: %s\n", crt_dir);
        exit(4);
    }
    struct dirent *entry;
    struct stat st;
    Data infos;
    while ((entry = readdir(dir)) != NULL) 
    {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;
        char fullPath[1024];
        snprintf(fullPath, sizeof(fullPath), "%s/%s",
                 crt_dir, entry->d_name);
        stat(fullPath, &st);
        if (S_ISDIR(st.st_mode)) 
        {
            strcpy(infos.name, entry->d_name);
            strcpy(infos.path, fullPath);
            infos.size  = st.st_size;
            infos.nivel = level;
            push(S, infos);
            getDirs(fullPath, S, level+1);
        }
    }
    closedir(dir);
}

// Salvare recursivă (task1)
void saveStructureRecursively(const char *path, int level, FILE *out) 
{
    DIR *d = opendir(path);
    if (!d) 
    {
        fprintf(out, "%*sEroare la deschiderea directorului: %s\n",
                level*4, "", path);
        return;
    }
    struct dirent *entry;
    struct stat st;
    char full[1024];
    while ((entry = readdir(d)) != NULL) 
    {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;
        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);
        stat(full, &st);
        long long size = S_ISDIR(st.st_mode) ? 4096 : st.st_size;
        fprintf(out, "%*s%lld %s\n",
                level*4, "", size, entry->d_name);
        if (S_ISDIR(st.st_mode))
            saveStructureRecursively(full, level+1, out);
    }
    closedir(d);
}

char* formatSize(long long size, int human) 
{
    static char buf[32];
    if (human)
    {
        if (size == 4096)           return "4K";
        else if (size < 1024)       sprintf(buf, "%lld", size);
        else if (size < 1024*1024)  sprintf(buf, "%lldK", size/1024);
        else                        sprintf(buf, "%lldM", size/(1024LL*1024));
        return buf;
    } else 
    {
        sprintf(buf, "%lld", size);
        return buf;
    }
}


typedef struct {
    char *name;
    long long size;
    int is_dir;
} Entry;

static int cmp_entry(const void *p1, const void *p2) 
{
    const Entry *a = p1;
    const Entry *b = p2;
    return strcmp(a->name, b->name);
}

void util_ls(const char *dir_path, int depth, int max_depth, int human, FILE *out)
{
    DIR *dir = opendir(dir_path);
    if (!dir) 
    {
        fprintf(out, "%*sEroare la deschiderea directorului: %s\n",
                depth * 4, "", dir_path);
        return;
    }

    Entry *entries = NULL;
    size_t cap = 0, count = 0;
    struct dirent *ent;


    while ((ent = readdir(dir)) != NULL) 
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        if (count + 1 > cap) 
        {
            cap = cap ? cap * 2 : 16;
            entries = realloc(entries, cap * sizeof(*entries));
        }

        entries[count].name = malloc(strlen(ent->d_name) + 1);
        strcpy(entries[count].name, ent->d_name);

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, ent->d_name);
        struct stat st;
        if (stat(full_path, &st) == 0) 
        {
            entries[count].is_dir = S_ISDIR(st.st_mode);
            entries[count].size = entries[count].is_dir ? 4096 : st.st_size;
        } else 
        {
            entries[count].is_dir = 0;
            entries[count].size = 0;
        }
        count++;
    }
    closedir(dir);

    qsort(entries, count, sizeof(*entries), cmp_entry);

    for (size_t i = 0; i < count; i++) 
    {
        fprintf(out, "%*s%s %s\n",
                depth * 4, "",
                formatSize(entries[i].size, human),
                entries[i].name);
        if (entries[i].is_dir && (max_depth < 0 || depth + 1 <= max_depth)) 
        {
            char next_path[1024];
            snprintf(next_path, sizeof(next_path), "%s/%s", dir_path, entries[i].name);
            util_ls(next_path, depth + 1, max_depth, human, out);
        }
        free(entries[i].name);
    }
    free(entries);
}


char *util_cd(char *srcpath, char *dstpath) 
{
    
    char *orig_src = malloc(strlen(srcpath) + 1);
    char *orig_dst = malloc(strlen(dstpath) + 1);
    if (!orig_src || !orig_dst) 
    {
        free(orig_src);
        free(orig_dst);
        return NULL;
    }
    strcpy(orig_src, srcpath);
    strcpy(orig_dst, dstpath);

    size_t len = strlen(orig_src);
    if (len > 1 && orig_src[len - 1] == '/')
        orig_src[len - 1] = '\0';
    len = strlen(orig_dst);
    if (len > 1 && orig_dst[len - 1] == '/')
        orig_dst[len - 1] = '\0';

    char *s = orig_src;
    while (s[0] == '.' && s[1] == '/')
        s += 2;
    char *d = orig_dst;
    while (d[0] == '.' && d[1] == '/')
        d += 2;

    char *src_parts[256], *dst_parts[256];
    int n_src = 0, n_dst = 0;
    char *tok = strtok(s, "/");
    while (tok && n_src < 256)
    {
        src_parts[n_src++] = tok;
        tok = strtok(NULL, "/");
    }
    tok = strtok(d, "/");
    while (tok && n_dst < 256) 
    {
        dst_parts[n_dst++] = tok;
        tok = strtok(NULL, "/");
    }

    int common = 0;
    while (common < n_src && common < n_dst &&
           strcmp(src_parts[common], dst_parts[common]) == 0)
    {
        common++;
    }

    int up = n_src - common;
    int down = n_dst - common;

    size_t buf_len = up * 3 + 1;
    for (int i = common; i < n_dst; i++)
        buf_len += strlen(dst_parts[i]) + 1;
    char *res = malloc(buf_len + 1);
    if (!res) 
    {
        free(orig_src);
        free(orig_dst);
        return NULL;
    }

    char *p = res;
    if (up == 0 && down == 0)
    {
        p += sprintf(p, "./");
    }else
    {
        for (int i = 0; i < up; i++)
            p += sprintf(p, "../");
        for (int i = common; i < n_dst; i++)
            p += sprintf(p, "%s/", dst_parts[i]);
    }
    *p = '\0';

    free(orig_src);
    free(orig_dst);
    return res;
}

int main(int argc, char **argv) 
{

    if (!strcmp(argv[1], "task1")) 
    {
        Lista *argumente = NULL, *optiuni = NULL, *flags = NULL, *p;
        int fis_intrare_gasit = 0, fis_iesire_gasit = 0;
        char *in_path = NULL, out_path[100];
        const char *tok = NULL;
        char nume_fis[20], *s = NULL, tip_parametru;

        func(argc, argv, &argumente, &optiuni, &flags);

        FILE *in, *out;
        p = optiuni;
        while (p && (!fis_intrare_gasit || !fis_iesire_gasit)) 
        {
            if (!strcmp((char*)p->data.valoare, "--file") ||
                !strcmp((char*)p->data.valoare, "-f")) 
            {
                in_path = malloc(strlen("/home/mihai/proiect_pa/") +
                                 strlen(p->data.val_opt) + 1);
                strcpy(in_path, "/home/mihai/proiect_pa/");
                strcat(in_path, p->data.val_opt);
                fis_intrare_gasit = 1;
            }
            if (!strcmp((char*)p->data.valoare, "--out") ||
                !strcmp((char*)p->data.valoare, "-o")) 
            {
                strcpy(nume_fis, p->data.val_opt);
                tok = strtok(nume_fis, "/");
                strcpy(nume_fis, tok);
                tok = strtok(NULL, "/");
                fis_iesire_gasit = 1;
            }
            p = p->urm;
        }
        if (!fis_intrare_gasit || !fis_iesire_gasit) 
        {
            printf("Nu s-au introdus fisierele de intrare si iesire.\n\n");
            sterge_lista(&argumente);
            sterge_lista(&optiuni);
            sterge_lista(&flags);
            free(in_path);
            exit(1);
        }
        in = fopen(in_path, "rt");
        if (!in) 
        {
            printf("Nu s-a putut deschide fisierul de intrare.\n\n");
            sterge_lista(&argumente);
            sterge_lista(&optiuni);
            sterge_lista(&flags);
            free(in_path);
            exit(2);
        }
        strcpy(out_path, "/home/mihai/proiect_pa/out/");
        strcat(out_path, tok);
        out = fopen(out_path, "wt");
        if (!out) 
        {
            printf("Nu s-a putut deschide fisierul de output\n");
            sterge_lista(&argumente);
            sterge_lista(&optiuni);
            sterge_lista(&flags);
            free(in_path);
            exit(3);
        }
        s = malloc(512);
        if (!s) 
        {
            printf("Nu s-a putut aloca dinamic.\n");
            sterge_lista(&argumente);
            sterge_lista(&optiuni);
            sterge_lista(&flags);
            free(in_path);
            exit(9);
        }
        while (fgets(s, 512, in)) 
        {
            size_t len = strlen(s);
            if (len && s[len-1]=='\n') s[len-1]=0;
            tip_parametru = gasire_param(s);
            switch (tip_parametru) 
            {
                case 'a':
                    strcat(s, " ");
                    if (!eGol(&argumente)) 
                    {
                        strcat(s, (char*)argumente->data.valoare);
                        strcat(s, " ");
                        sterge_element_inceput(&argumente);
                    }
                    break;
                case 'o':
                    strcat(s, " ");
                    if (!eGol(&optiuni)) 
                    {
                        strcat(s, (char*)optiuni->data.val_opt);
                        strcat(s, " ");
                        sterge_element_inceput(&optiuni);
                    }
                    break;
                case 'f':
                    strcat(s, " ");
                    if (!eGol(&flags)) 
                    {
                        strcat(s, (char*)flags->data.val_opt);
                        strcat(s, " ");
                        sterge_element_inceput(&flags);
                    }
                    break;
                default:
                    break;
            }
            strcat(s, "\n");
            fwrite(s, strlen(s), 1, out);
        }
        #if AFIS==1||AFIS==4
            p = argumente;
            while (p) 
            {
                printf("%c %s\n", p->data.tip, (char*)p->data.valoare);
                p = p->urm;
            }
        #endif
        #if AFIS==2||AFIS==4
            p = flags;
            while (p) 
            {
                printf("%s %c %s\n", (char*)p->data.valoare, p->data.tip, p->data.val_opt);
                p = p->urm;
            }
        #endif
        #if AFIS==3||AFIS==4
            p = optiuni;
            while (p) 
            {
                printf("%c %s %s\n", p->data.tip, (char*)p->data.valoare, p->data.val_opt);
                p = p->urm;
            }
        #endif
        sterge_lista(&argumente);
        sterge_lista(&optiuni);
        sterge_lista(&flags);
        free(in_path);
        free(s);
        return 0;
    }
    else if (!strcmp(argv[1], "task2")) 
    {    
        char *inputDir = argv[2];
        int recurse = 0, human = 0;
        char *outPath = NULL;
        for (int i = 3; i < argc; i++) 
        {
            if (!strncmp(argv[i], "--out=", 6)) 
            {
                outPath = argv[i] + 6;
            }
            else if (!strcmp(argv[i], "-o") && i+1<argc) 
            {
                outPath = argv[++i];
            }
            else if (argv[i][0]=='-' && argv[i][1]!='-')
            {
                for (int j=1; j<(int)strlen(argv[i]); j++)
                {
                    if (argv[i][j]=='r') recurse=1;
                    if (argv[i][j]=='h') human=1;
                }
            }
        }

        if (!outPath) 
        {
            fprintf(stderr, "Lipseste --out=<fisier>\n");
            return 1;
        }
        char cop1[100], *cop2, cop3[100] = "out/";
        strcpy(cop1, outPath);
        cop2 = strtok(cop1, "/");
        cop2 = strtok(NULL, "/");
        strcat(cop3, cop2);
        outPath = cop3;
        FILE *out = fopen(outPath, "wt");
        if (!out) 
        {
            printf("fopen nu a functionat");
            return 3;
        }
        fprintf(out, "%s .\n", formatSize(4096, human));
        fprintf(out, "%s ..\n", formatSize(4096, human));
        if (recurse)
            util_ls(inputDir, 0, -1, human, out);
        else
            util_ls(inputDir, 0,  0, human, out);
        fclose(out);
        return 0;
    }
    else if (!strcmp(argv[1], "task3")) 
    {
        char *src = argv[2];
        char *dst = argv[3];
        char *outPath = NULL;
        for (int i = 4; i < argc; i++) 
        {
            if (!strncmp(argv[i], "--out=", 6)) 
            {
                outPath = argv[i] + 6;
            }
            else if (!strcmp(argv[i], "-o") && i+1<argc) 
            {
                outPath = argv[++i];
            }
        }
        if (!outPath) 
        {
            fprintf(stderr, "Lipseste --out=<fisier>\n");
            return 1;
        }
        FILE *out = fopen(outPath, "wt");
        if (!out) 
        {
            printf("fopen nu a functionat");
            return 3;
        }
        char *rel = util_cd(src, dst);
        fprintf(out, "%s\n", rel);
        free(rel);
        fclose(out);
        return 0;
    }
}
