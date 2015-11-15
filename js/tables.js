var options = {packages: ['corechart','columnchart'], callback : drawChart};
google.load('visualization', '1', options);
// Load the Visualization API and the piechart package.
//google.load('visualization', '1.0', {'packages':['corechart','columnchart']});

// Set a callback to run when the Google Visualization API is loaded.
//google.setOnLoadCallback(drawChart);

// Callback that creates and populates a data table,
// instantiates the pie chart, passes in the data and
// draws it.
function drawChart() {

    // Create the data table.
    var data1 = google.visualization.arrayToDataTable([
        ['Library', 'time/s', { role: 'style' }],
        ['FADBAD++', 0.575, 'blue'],
        ['SACADO', 0.542, 'blue'],
        ['FunG', 0.517, 'orange'],
        ['Manual', 0.507, 'gray']
    ]);
    
    var data2 = google.visualization.arrayToDataTable([
        ['Library', 'time/s', { role: 'style' }],
        ['FADBAD++', 1.412, 'blue'],
        ['FunG', 1.364, 'orange'],
        ['Manual', 1.325, 'gray']
    ]);

    var data3 = google.visualization.arrayToDataTable([
        ['Library', 'time/s', { role: 'style' }],
        ['FADBAD++', 0.581, 'blue'],
        ['SACADO', 0.586, 'blue'],
        ['FunG', 0.499, 'orange'],
        ['Manual', 0.475, 'gray']
    ]);

    var data4 = google.visualization.arrayToDataTable([
        ['Function','Armadillo','DUNE','Eigen'],
        ['update', 2.587   , 2.512    , 2.159],
        ['d0'    , 0.000071, 0.000040 , 0.000064],
        ['d1'    , 0.593   , 0.601    , 0.275],
        ['d2'    , 3.332   , 3.404    , 2.234],
        ['d3'    , 19.227  , 15.852   , 12.026]
    ]);

    var data5 = google.visualization.arrayToDataTable([
        ['Function','Armadillo','DUNE','Eigen'],
        ['update', 2.982   , 2.780    , 2.836],
        ['d0'    , 0.000042, 0.000043 , 0.000049],
        ['d1'    , 1.247   , 0.988    , 1.035],
        ['d2'    , 7.080   , 6.044    , 5.880],
        ['d3'    , 40.810  , 35.792   , 33.987]
    ]);
    
    
    // Set chart options
    var options = {title:'Comparison with different AD implementations',
                    width:500,
                    height:300,
                    legend : { position:"none"},
                    vAxis: {
                        title: 'time/s'
                    }
                    };

        var options_muscle = {title:'Performance for a model of muscle tissue',
                    width:500,
                    height:300,
                    colors: ['blue','orange','gray'],
                    vAxis: {
                        title: 'time/s'
                    }
                    };

                    
    // Instantiate and draw our chart, passing in some options.
    var chart1 = new google.visualization.ColumnChart(document.getElementById('chart1_div'));
    chart1.draw(data1, options);
    var chart2 = new google.visualization.ColumnChart(document.getElementById('chart2_div'));
    chart2.draw(data2, options);
    var chart3 = new google.visualization.ColumnChart(document.getElementById('chart3_div'));
    chart3.draw(data3, options);
    var chart4 = new google.visualization.ColumnChart(document.getElementById('chart4_div'));
    chart4.draw(data4, options_muscle);
    var chart5 = new google.visualization.ColumnChart(document.getElementById('chart5_div'));
    chart5.draw(data5, options_muscle);
    
    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
    MathJax.Hub.Queue( function(){
        var contentHeight = $('.main-content').height();
        var headerOffset = 150;
        $('.background-image-container').height(contentHeight + headerOffset);
    });
}