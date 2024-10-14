#ifndef SINTATICA_H
#define SINTATICA_H


//Verifica se uma duração é válida e está escrita direito.
int valid_duration (char *duration);

//Verifica se um email está escrito direito.
int valid_email_string (char *email);

//Verifica se uma data de nascimento é válida e está escrita direito.
int valid_date (char *date);

//Verifica se uma subscription_type está escrita direito.
int valid_subscription (char *subs_type);

//Verifica se os campos que têm de ser sintáticamente validados de um dado utilizador estão direito/válidos.
int valid_user_sintatic (char *email, char *date, char sub_type);

#endif