
document.addEventListener('DOMContentLoaded', function() {
    

    const linksNavegacao = document.querySelectorAll('header a[href^="#"]');

    linksNavegacao.forEach(link => {
        link.addEventListener('click', function(evento) {
      
            evento.preventDefault();

            const destinoId = this.getAttribute('href');
            const secçãoDestino = document.querySelector(destinoId);

            if (secçãoDestino) {
                secçãoDestino.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });

    const botoesManejo = document.querySelectorAll('#manejo button');
    botoesManejo.forEach(botao => {
        botao.addEventListener('click', function() {
            alert('A carregar informações sobre: ' + this.innerText);
        });
    });
});