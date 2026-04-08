/*
 * Online Ad Slot Allocation - DAA Project
 * Algorithms: Greedy (Activity Selection), DP (Weighted Interval Scheduling), Knapsack
 * Input: JSON via stdin
 * Output: JSON via stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_ADS 100
#define MAX_CAPACITY 10000

typedef struct {
    int id;
    int start;
    int end;
    int bid;
    char name[32];
} Ad;

typedef struct {
    int selected[MAX_ADS];
    int count;
    int totalProfit;
    int totalTime;
    double timeMs;
} Result;

/* ---- Simple JSON parser ---- */
int parse_int(const char *json, const char *key) {
    char search[64];
    snprintf(search, sizeof(search), "\"%s\"", key);
    const char *p = strstr(json, search);
    if (!p) return -1;
    p += strlen(search);
    while (*p == ':' || *p == ' ') p++;
    return atoi(p);
}

int parse_ads(const char *json, Ad *ads) {
    int count = 0;
    const char *p = strstr(json, "\"ads\"");
    if (!p) return 0;
    p = strchr(p, '[');
    if (!p) return 0;
    p++;

    while (*p && *p != ']' && count < MAX_ADS) {
        const char *obj_start = strchr(p, '{');
        if (!obj_start) break;
        const char *obj_end = strchr(obj_start, '}');
        if (!obj_end) break;

        char obj[256];
        int len = (int)(obj_end - obj_start + 1);
        if (len >= (int)sizeof(obj)) len = sizeof(obj) - 1;
        strncpy(obj, obj_start, len);
        obj[len] = '\0';

        ads[count].id = count + 1;
        ads[count].start = parse_int(obj, "start");
        ads[count].end = parse_int(obj, "end");
        ads[count].bid = parse_int(obj, "bid");

        /* Parse name field */
        const char *nkey = strstr(obj, "\"name\"");
        if (nkey) {
            nkey = strchr(nkey, ':');
            if (nkey) {
                nkey++;
                while (*nkey == ' ') nkey++;
                if (*nkey == '"') {
                    nkey++;
                    int ni = 0;
                    while (*nkey && *nkey != '"' && ni < 31) {
                        ads[count].name[ni++] = *nkey++;
                    }
                    ads[count].name[ni] = '\0';
                }
            }
        }
        if (strlen(ads[count].name) == 0) {
            snprintf(ads[count].name, sizeof(ads[count].name), "Ad_%d", count + 1);
        }

        count++;
        p = obj_end + 1;
    }
    return count;
}

/* ---- Comparison functions ---- */
int cmp_by_end(const void *a, const void *b) {
    return ((Ad*)a)->end - ((Ad*)b)->end;
}

int cmp_by_ratio(const void *a, const void *b) {
    double ra = (double)((Ad*)a)->bid / ((Ad*)a)->end - ((Ad*)a)->start;
    double rb = (double)((Ad*)b)->bid / ((Ad*)b)->end - ((Ad*)b)->start;
    if (rb > ra) return 1;
    if (rb < ra) return -1;
    return 0;
}

/* ---- Greedy Activity Selection (by end time) ---- */
Result greedy_activity(Ad *ads, int n) {
    Result res;
    memset(&res, 0, sizeof(res));

    Ad sorted[MAX_ADS];
    memcpy(sorted, ads, n * sizeof(Ad));
    qsort(sorted, n, sizeof(Ad), cmp_by_end);

    int lastEnd = -1;
    for (int i = 0; i < n; i++) {
        if (sorted[i].start >= lastEnd) {
            res.selected[res.count++] = sorted[i].id;
            res.totalProfit += sorted[i].bid;
            res.totalTime += sorted[i].end - sorted[i].start;
            lastEnd = sorted[i].end;
        }
    }
    return res;
}

/* ---- Greedy by Bid/Duration Ratio ---- */
Result greedy_ratio(Ad *ads, int n) {
    Result res;
    memset(&res, 0, sizeof(res));

    Ad sorted[MAX_ADS];
    memcpy(sorted, ads, n * sizeof(Ad));

    /* Bubble sort by bid descending */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double r1 = (double)sorted[j].bid / (sorted[j].end - sorted[j].start + 1);
            double r2 = (double)sorted[j+1].bid / (sorted[j+1].end - sorted[j+1].start + 1);
            if (r1 < r2) {
                Ad tmp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1] = tmp;
            }
        }
    }

    /* Select non-overlapping greedily by ratio */
    int used[MAX_ADS];
    memset(used, 0, sizeof(used));

    for (int i = 0; i < n; i++) {
        int overlap = 0;
        for (int j = 0; j < res.count; j++) {
            /* Find the ad with id == res.selected[j] */
            for (int k = 0; k < n; k++) {
                if (ads[k].id == res.selected[j]) {
                    if (sorted[i].start < ads[k].end && sorted[i].end > ads[k].start) {
                        overlap = 1;
                    }
                    break;
                }
            }
            if (overlap) break;
        }
        if (!overlap) {
            res.selected[res.count++] = sorted[i].id;
            res.totalProfit += sorted[i].bid;
            res.totalTime += sorted[i].end - sorted[i].start;
        }
    }
    return res;
}

/* ---- DP: Weighted Interval Scheduling ---- */
int latest_non_conflict(Ad *sorted, int j) {
    for (int i = j - 1; i >= 0; i--) {
        if (sorted[i].end <= sorted[j].start) return i;
    }
    return -1;
}

Result dp_weighted_interval(Ad *ads, int n) {
    Result res;
    memset(&res, 0, sizeof(res));

    Ad sorted[MAX_ADS];
    memcpy(sorted, ads, n * sizeof(Ad));
    qsort(sorted, n, sizeof(Ad), cmp_by_end);

    int dp[MAX_ADS + 1];
    dp[0] = 0;
    for (int j = 1; j <= n; j++) {
        int p = latest_non_conflict(sorted, j - 1);
        int include = sorted[j-1].bid + (p >= 0 ? dp[p+1] : 0);
        dp[j] = include > dp[j-1] ? include : dp[j-1];
    }

    res.totalProfit = dp[n];

    /* Traceback */
    int j = n;
    while (j > 0) {
        int p = latest_non_conflict(sorted, j - 1);
        int include = sorted[j-1].bid + (p >= 0 ? dp[p+1] : 0);
        if (include > dp[j-1]) {
            res.selected[res.count++] = sorted[j-1].id;
            res.totalTime += sorted[j-1].end - sorted[j-1].start;
            j = p + 1;
        } else {
            j--;
        }
    }
    return res;
}

/* ---- DP: 0/1 Knapsack (capacity = total time window) ---- */
Result knapsack(Ad *ads, int n, int capacity) {
    Result res;
    memset(&res, 0, sizeof(res));

    if (capacity <= 0 || capacity > MAX_CAPACITY) capacity = 100;

    /* dp[i][w] = max profit using first i ads with capacity w */
    static int dp[MAX_ADS + 1][MAX_CAPACITY + 1];
    for (int i = 0; i <= n; i++)
        for (int w = 0; w <= capacity; w++)
            dp[i][w] = 0;

    for (int i = 1; i <= n; i++) {
        int duration = ads[i-1].end - ads[i-1].start;
        int bid = ads[i-1].bid;
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i-1][w];
            if (duration <= w && dp[i-1][w-duration] + bid > dp[i][w]) {
                dp[i][w] = dp[i-1][w-duration] + bid;
            }
        }
    }

    res.totalProfit = dp[n][capacity];

    /* Traceback */
    int w = capacity;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            res.selected[res.count++] = ads[i-1].id;
            res.totalTime += ads[i-1].end - ads[i-1].start;
            w -= (ads[i-1].end - ads[i-1].start);
        }
    }
    return res;
}

/* ---- Output JSON ---- */
void print_result_json(Result *res, Ad *ads, int n, const char *method, const char *method_label) {
    printf("{");
    printf("\"method\":\"%s\",", method);
    printf("\"methodLabel\":\"%s\",", method_label);
    printf("\"totalProfit\":%d,", res->totalProfit);
    printf("\"totalTime\":%d,", res->totalTime);
    printf("\"count\":%d,", res->count);
    printf("\"schedule\":[");
    for (int i = 0; i < res->count; i++) {
        if (i > 0) printf(",");
        /* Find the ad */
        for (int j = 0; j < n; j++) {
            if (ads[j].id == res->selected[i]) {
                printf("{\"id\":%d,\"name\":\"%s\",\"start\":%d,\"end\":%d,\"bid\":%d}",
                    ads[j].id, ads[j].name, ads[j].start, ads[j].end, ads[j].bid);
                break;
            }
        }
    }
    printf("]}");
}

int main() {
    /* Read all stdin */
    char input[65536];
    int len = 0;
    int c;
    while ((c = getchar()) != EOF && len < (int)sizeof(input) - 1) {
        input[len++] = (char)c;
    }
    input[len] = '\0';

    /* Parse method */
    char method[32] = "greedy";
    const char *mp = strstr(input, "\"method\"");
    if (mp) {
        mp = strchr(mp, ':');
        if (mp) {
            mp++;
            while (*mp == ' ' || *mp == '"') mp++;
            int mi = 0;
            while (*mp && *mp != '"' && mi < 31) method[mi++] = *mp++;
            method[mi] = '\0';
        }
    }

    /* Parse capacity */
    int capacity = parse_int(input, "capacity");
    if (capacity <= 0) capacity = 100;

    /* Parse ads */
    Ad ads[MAX_ADS];
    int n = parse_ads(input, ads);

    if (n == 0) {
        printf("{\"error\":\"No ads provided\"}");
        return 1;
    }

    /* All results */
    if (strcmp(method, "all") == 0) {
        Result r_greedy = greedy_activity(ads, n);
        Result r_ratio  = greedy_ratio(ads, n);
        Result r_dp     = dp_weighted_interval(ads, n);
        Result r_ks     = knapsack(ads, n, capacity);

        printf("{\"results\":[");
        print_result_json(&r_greedy, ads, n, "greedy", "Greedy (End Time)");
        printf(",");
        print_result_json(&r_ratio, ads, n, "ratio", "Greedy (Bid/Duration Ratio)");
        printf(",");
        print_result_json(&r_dp, ads, n, "dp", "DP Weighted Interval");
        printf(",");
        print_result_json(&r_ks, ads, n, "knapsack", "Knapsack DP");
        printf("]}");
    } else if (strcmp(method, "greedy") == 0) {
        Result r = greedy_activity(ads, n);
        print_result_json(&r, ads, n, "greedy", "Greedy (End Time)");
    } else if (strcmp(method, "ratio") == 0) {
        Result r = greedy_ratio(ads, n);
        print_result_json(&r, ads, n, "ratio", "Greedy (Bid/Duration Ratio)");
    } else if (strcmp(method, "dp") == 0) {
        Result r = dp_weighted_interval(ads, n);
        print_result_json(&r, ads, n, "dp", "DP Weighted Interval");
    } else if (strcmp(method, "knapsack") == 0) {
        Result r = knapsack(ads, n, capacity);
        print_result_json(&r, ads, n, "knapsack", "Knapsack DP");
    } else {
        printf("{\"error\":\"Unknown method\"}");
        return 1;
    }

    return 0;
}
