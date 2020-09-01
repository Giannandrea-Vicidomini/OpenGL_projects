


function checkFormat(str){
    var res = false
    var c = str.charAt(0);
    if(c!='0'){
        return res;
    }

    for(var i=0;i<str.length;i++){
        c = str.charAt(i);
     
        if(res == false && c =='1' ){
            res = true;
        }
        if(res == true && c =='0'){
            res = false;
        }
    }

    return res;

}

function isNonRegular(str){

    var size = str.length;
    if(!checkFormat(str)){
        return false;
    }
    var zeros = 0;
    var ones = 0;

    for(let l of str){
        if(l=='0')zeros = zeros+1;
        if(l=='1')ones = ones+1;
    }

    if(ones == zeros){
        return true;
    
    }
    else{
        return false;
    }
    
}


function inputMDT(str){
    var language = ['a','b','c','d'];
    
    return language.includes(str);

}

function f(M,w){

    
    function R(x){

        if(isNonRegular(x)){
            return true;
        }  
        else{
            if( M(w) ){ //if M accepts w
                return true;

            }
            else{
                return false;
            }
        }
    }

    return R;

}

var outputMDT1 = f(inputMDT,'a');  // " M " accepts w
console.error(outputMDT1("000111")) // true
console.error(outputMDT1("01000"));  // true
console.error(outputMDT1("pippo"));   // true
console.error(outputMDT1("qualsiasi stringa")); // true

console.error(" ");

var outputMDT2 = f(inputMDT,'gnigni'); // " M " rejects w 
console.error(outputMDT2("01000")); // false
console.error(outputMDT2("pippo"));  // false
console.error(outputMDT2("qualsiasi stringa")); // false
console.error(outputMDT2("000111")) // true





