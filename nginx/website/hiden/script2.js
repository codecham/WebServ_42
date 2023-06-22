"use strict";

window.onload = function() 
{
    var start = ([[14,9], [13,9], [12,9]]);
    var appleStart_x = getRandomInt(2,29);
    var appleStart_y = getRandomInt(2,19);
    var delay = 100;
    var canvasWidth = 900;
    var canvasHeight = 600;
    var score = 0;
    var blockSize = 30;
    var appleSize = 30;
    var apple;
    var ctx;
    var xCoord = 2;
    var yCoord = 2;
    var snakee;
    var gameOver = false;
    var timeout;
    
 //----------------------------------------------------------------------------------------------------   
    function init()
    {    
        var canvas = document.createElement('canvas');
        canvas.width = canvasWidth;
        canvas.height = canvasHeight;
        canvas.style.border = "30px solid black";
        canvas.style.margin = "50px auto";
        canvas.style.display = "block";
        canvas.style.backgroundColor = "#009933";
        document.body.appendChild(canvas);
        ctx = canvas.getContext('2d');
        snakee = new Snake(start, "right");
        apple = new Apple([appleStart_x, appleStart_y]);
        refreshCanvas();
    }
 //----------------------------------------------------------------------------------------------------    
    function refreshCanvas()
    {
//        console.log(delay);
        ctx.clearRect(0,0, canvasWidth, canvasHeight);
        snakee.advance();
        if(snakee.body[0][0] === apple.position[0] && snakee.body[0][1] === apple.position[1])
        {
            apple.position[0] = getRandomInt(2,29);
            apple.position[1] = getRandomInt(2,19);
            snakee.body.push(snakee.body[snakee.length-1]++);
            score++;
            console.log("score: " + score);
        }
        drawScore();
        snakee.draw();
        apple.draw();
        if(gameOver == true)
        {
            game_over();
        }
        else
        {
            timeout = setTimeout(refreshCanvas, delay); 
        }
            
    }
  //----------------------------------------------------------------------------------------------------   
    function drawBlock(ctx, position, typeSize)
    {
        var x = position[0] * typeSize;
        var y = position[1] * typeSize;
        ctx.fillRect(x,y,typeSize,typeSize);
    }
  //----------------------------------------------------------------------------------------------------   
    function Snake(body, direction)
    {
        this.body = body;
        this.direction = direction;
        this.draw = function()
        {
            ctx.save();
         //   ctx.fillStyle = "#62CF89";
            for(var i = 0 ; i < this.body.length ; i++)
            {
                if(i == 0)
                {
                    ctx.fillStyle = "09FB8D";
                    drawBlock(ctx, this.body[i], blockSize);
                        
                }
                else
                {
                    // ctx.fillStyle = "#000000";
                    ctx.fillStyle = "#62CF89";
                    drawBlock(ctx, this.body[i], blockSize);
                }
                drawBlock(ctx, this.body[i], blockSize);
            }
            ctx.restore();  
        };
        this.advance = function()
        {
            var nextPosition = this.body[0].slice();
            switch(this.direction)
            {
                case "left":
                    nextPosition[0]--;
                    break;
                case "right":
                    nextPosition[0]++;
                    break;
                case "up":
                    nextPosition[1]--;
                    break;
                case "down":
                    nextPosition[1]++;
                    break;
                default:
                    throw("invalid direction");
                
            }
            for(var i = 1 ; i < this.body.length ; i++)
            {
                console.log(nextPosition + this.body);
                if(nextPosition[0] == this.body[i][0] && nextPosition[1] == this.body[i][1])
                { 
                    gameOver = true;
                }
            } 
            this.body.unshift(nextPosition);
            this.body.pop();
            if(nextPosition[0] < 0 || nextPosition[0] > (((canvasWidth/blockSize)-1)))
            {
                gameOver = true;      
            }
            if(nextPosition[1] < 0 || nextPosition[1] > (((canvasHeight/blockSize)-1)))
            {
                gameOver = true;
            }
        };       
        this.setDirection = function(newDirection)
        {
            var allowedDirection;
            switch(this.direction)
            {
                case "left":
                    allowedDirection = ["up","down"];
                    break;
                case "right":
                    allowedDirection = ["up","down"];
                    break;
                case "up":
                    allowedDirection = ["right", "left"];
                    break;
                case "down":
                    allowedDirection = ["right", "left"];
                    break;
                default:
                    throw("invalid direction");
            }
            if(allowedDirection.indexOf(newDirection) > -1)
            {
                this.direction = newDirection;  
            }
        };
    }
//----------------------------------------------------------------------------------------------------     
    document.onkeydown = function handleKeyDown(e)
    {
        var key = e.keyCode;
        var newDirection;
        switch(key)
        {
            case 37:
                newDirection = "left";
                break;
            case 38:
                newDirection = "up";
                break;
            case 39:
                newDirection = "right";
                break;
            case 40:
                newDirection = "down";
                break;
            case 32:
                if(gameOver == true)
                {
                    restart();
                    return;
                }
                else
                break;
            default:
                return;
        }
        snakee.setDirection(newDirection);
    }
//----------------------------------------------------------------------------------------------------
    
    function getRandomInt(min, max)
    {
        var random = Math.random() * (max - min) + min;
        return Math.floor(random);
    }
//----------------------------------------------------------------------------------------------------
    function Apple(position)
    {
        this.position = position;
        this.draw = function()
        {
            ctx.save();
            ctx.fillStyle = "#ff0000";
            ctx.beginPath();
            var radius = blockSize/2;
            var x = position[0] * blockSize + radius;
            var y = position[1] * blockSize + radius;
            ctx.arc(x,y, radius, 0, Math.PI*2, true);
            ctx.fill();
            ctx.restore();
            
        }
        
    }
    
//----------------------------------------------------------------------------------------------------
    function game_over()
    {
        ctx.save();
        ctx.font = "bold 25px sans-serif";
//        ctx.fillStyle = "black";
//        ctx.textAlign = "center";
//        ctx.textBaseline = "middle";
        ctx.fillText("Game Over!", 5, 20);
        ctx.fillText("Your score: " + score, 5, 40);
        ctx.fillText("Press space for restart", 5, 60);
        ctx.restore();
    }
//----------------------------------------------------------------------------------------------------
    function restart()
    {
        score = 0;
        snakee = new Snake([[14,9], [13,9], [12,9]], "right");
        apple = new Apple([getRandomInt(2,29),getRandomInt(2,19)]);
        gameOver = false;
        delay = 100;
        clearTimeout(timeout);
        refreshCanvas();
    }
//----------------------------------------------------------------------------------------------------
    function drawScore()
    {
        var centre_x = canvasWidth/2;
        var centre_y = canvasHeight/2;
        
        ctx.save();
        ctx.font = "bold 200px sans-serif";
        ctx.fillStyle = "black";
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(score.toString(), centre_x, centre_y);
        ctx.restore();
    }
//----------------------------------------------------------------------------------------------------

    
    init();
}