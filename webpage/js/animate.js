/*var animate;
var container;
var position;
window.onload = function () {
    animate = document.getElementById('animate');
    container = document.getElementById('container');
};



function anim(event){
        //position = $("#animate").position();
        alert("Hello");
if (event.keyCode == 37) {
       // elem.style.left = (elem.style.left - 1) + 'px';
       animate.style.left = 100 - 2 + 'px';
    }
else if (event.keyCode == 39) {
       animate.style.left = 100 + 2 + 'px';
    }
}

   document.onkeydown = anim;
*/

    
    /*
     var pos = 0;
    var id = setInterval(frame, 5);
    function frame() {
        if (pos == 350) {
            clearInterval(id);
        } else {
            pos++;
            elem.style.top = pos + 'px';
            elem.style.left = pos + 'px';
        }

*/
var animate;
var container;
var animateLeft;
window.onload = function () {
    animate = document.getElementById('animate');
    container = document.getElementById('container');
    //var centerCoord =  animate.getBoundingClientRect();
   // animateLeft = centerCoord.left;
   // animateTop = centerCoord.right;
    style = window.getComputedStyle(animate);
    vertCenter = parseInt(style.getPropertyValue('top'));
    horiCenter = parseInt(style.getPropertyValue('left'));
    animateTop = vertCenter;
    animateLeft = horiCenter;
    //animate.style.position = 'absolute';
};

function anim(e){

    //left (A)
    if (e.keyCode==65){
        /*animateLeft -=2;
        animate.style.left = animateLeft + 'px';
        if (animateLeft <= 0){
            animateLeft +=2;
        }*/
        animateLeft = animateLeft-100;
        animate.style.left = animateLeft + 'px';
    }
    //down (S)
    if (e.keyCode == 83){
        //for slow motion
        //animateTop -=2;
        //animate.style.top = animateTop + 'px';
        //for fast motion
        animateTop = animateTop+100;
        animate.style.top = animateTop + 'px';
    }
    //right (D)
    if (e.keyCode == 68){
       /* animateLeft +=2;
        animate.style.left = animateLeft + 'px';*/
        animateLeft = animateLeft+100;
        animate.style.left = animateLeft + 'px';

    }

    //up (W)
    if (e.keyCode == 87){
        /*animateTop -=2;
        animate.style.top = animateTop + 'px';
                if (animateTop <= 0){
            animateTop +=2;
        }*/
        animateTop = animateTop-100;
        animate.style.top = animateTop + 'px';
    }

animateTop=vertCenter;
animateLeft=horiCenter;  
}

function reset() {
animateTop=vertCenter;
animateLeft=horiCenter;  
animate.style.left = animateLeft + 'px';
 animate.style.top = animateTop + 'px';  
}

$(document).ready(function(){
       $(document).keyup(function(){
        reset();
    });
   document.onkeydown = anim;

 
});

