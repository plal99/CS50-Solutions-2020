#include <cs50.h>
#include <string.h>
#include <stdio.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool check_cycle(int loser, int winner);
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }
    // printf("Preferences array :- \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++) // Debug PREFERENCES
    //     {
    //         printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    add_pairs();
    // printf("Unsorted pair values :- \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     printf("%i ", pairs[i].winner); // Debug ADD_PAIRS
    //     printf("%i ", pairs[i].loser);
    //     printf("\n");
    // }
    // printf("\n");
    sort_pairs();
    // printf("Sorted pair values :- \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     printf("%i ", pairs[i].winner); // Debug SORT_PAIRS
    //     printf("%i ", pairs[i].loser);
    //     printf("\n");
    // }
    // printf("\n");
    lock_pairs();
    // printf("Locked pair values :-\n");
    // for (int i = 0; i < pair_count; i++)
    // {
    //     for (int j = 0; j < pair_count; j++) // Debug LOCKED_PAIRS
    //     {
    //         printf("%i ", locked[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        preferences[ranks[i]][ranks[i + 1]] += 1;
    }
    preferences[ranks[0]][ranks[candidate_count - 1]] += 1;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i != j)
            {
                if (preferences[i][j] > preferences[j][i])
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                    pair_count++;
                }
                // else if (preferences[i][j] < preferences[j][i])
                // {
                //     pairs[pair_count].loser = i;
                //     pairs[pair_count].winner = j;
                //     pair_count++;
                // }
                else
                {
                    continue;
                }
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            if (i != j)
            {
                if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[i].winner][pairs[i].loser]) //decreasing order sort
                {
                    temp = pairs[j];
                    pairs[j] = pairs[i];
                    pairs[i] = temp;
                }
            }
        }
    }
}
// Lock pairs into the candidate graph in order, without creating cycles

void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        bool cycle = check_cycle(pairs[i].loser, pairs[i].winner);
        if (!cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    bool valid;
    for (int i = 0; i < candidate_count; i++)
    {
        valid = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                valid = true;
                break;
            }
        }
        if (valid == false)
        {
            printf("%s\n", candidates[i]);
        }
        else if (valid == true)
        {
            continue;
        }
    }
}

// check for cycle in the locked pairs
bool check_cycle(int loser, int winner)
{
    if (locked[loser][winner]) // base case :- if loser is locked to winner
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[i][winner]) // if loser is winner in some other case
        {
            return check_cycle(loser, i);
        }
    }
    return false;
}
