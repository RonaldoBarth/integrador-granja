printf("\n=== Lista de Clientes ===\n");
                for (int i = 0; i < qtdClientes; i++) {
                    printf("Código: %d\n", clientes[i].codC);
                    printf("Nome: %s\n", clientes[i].nome);
                    printf("Nome Social: %s\n", clientes[i].nomeSocial);
                    printf("CPF: %s\n", clientes[i].cpf);
                    printf("Rua: %s\n", clientes[i].rua);
                    printf("Bairro: %s\n", clientes[i].bairro);
                    printf("Número: %s\n", clientes[i].numero);
                    printf("-----------------------------\n");
                }
                break;