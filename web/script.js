const container = document.querySelector('.container');
const search = document.querySelector('.search-box button');
const weatherBox = document.querySelector('.weather-box');
const weatherDetails = document.querySelector('.weather-details');
const error404 = document.querySelector('.not-found');
const cityHide = document.querySelector('.city-hide');


search.addEventListener ('click', () => {

    const APIKey = 'a652c91a816d969a67f72bbfec244d4a';
    const city = document.querySelector('.search-box input').value;  

    if (city == '')
        return;

    fetch (`https://api.openweathermap.org/data/2.5/weather?q=${city}&units=metric&appid=${APIKey}`).then(response => response.json() ).then(json => {
        
        if (json.cod == '404') {
            // cityHide.textContent = city;
            container.style.height = '400px';
            weatherBox.classList.remove('active');
            weatherDetails.classList.remove('active');
            error404.classList.add('active');
            return;
        }
    
        const image = document.querySelector ('.weather-box img');
        const temperature = document.querySelector ('.weather-box .temperature');
        const description = document.querySelector ('.weather-box .description');
        const humidity = document.querySelector ('.weather-details .humidity span');
        const wind = document.querySelector ('.weather-details .wind span');

        if (cityHide.textContent == city) {
            return;
        }
        else {
            cityHide.textContent = city;

            container.style.height = '555px';
            container.classList.add ('active');
            weatherBox.classList.add('active');
            weatherDetails.classList.add('active');
            error404.classList.remove('active');

            setTimeout (() => {
                container.classList.add ('active');  
            }, 2500 );

            switch (json.weather[0].main) {
                case 'Clear':
                    image.src = 'images/clear.png';
                    break;
                case 'Rain':
                    image.src = 'images/rain.png';
                    break;
                case 'Snow':
                    image.src = 'images/snow.png';
                    break;
                case 'Clouds':
                    image.src = 'images/cloud.png';
                    break;
                case 'Mist':
                    image.src = 'images/mist.png';
                    break;
                case 'Haze':
                    image.src = 'images/mist.png';
                    break;
    
                default:
                    image.src = 'images/mist.png'
                    
            }
    
            temperature.innerHTML = `${parseInt(json.main.temp)}<span>℃</span>`;
            description.innerHTML = `${json.weather[0].description}`;
            humidity.innerHTML = `${json.main.humidity}%`;
            wind.innerHTML = `${parseInt(json.wind.speed)}Km/h`;


        }
    });
    
});
