var items = [
    ["Google", "123@gmail.com", "David", "Martinez"],
    ["Yahoo", "guy@yahoo.com", "Mr", "Robot"],
    ["Outlook", "girl@outlook.com", "Cho", "Chang"]
];

// print items
function generateItems() {
    let text = "";
    for (let i = 0; i < items.length; i++) {
        text += `   <div class="entry">
                        <input id="${i}id" type="checkbox">
                        <div>
                            <h1 id="${i}org">${items[i][0]}</h1>
                            <img onclick="copyItem('${i}org')" src="copy.png" alt="copy_icon">
                        </div>
                        <div>
                            <h1 id="${i}mail">${items[i][1]}</h1>
                            <img onclick="copyItem('${i}mail')" src="copy.png" alt="copy_icon">
                        </div>
                        <div>
                            <h1 id="${i}user">${items[i][2]}</h1>
                            <img onclick="copyItem('${i}user')" src="copy.png" alt="copy_icon">
                        </div>
                        <div>
                            <h1 id="${i}pass">${items[i][3]}</h1>
                            <img onclick="copyItem('${i}pass')" src="Copy.png" alt="copy_icon">
                        </div>
                    </div>`
    }
    return text;
}

// copy text-field to clipboard
function copyItem(entryId) {
    navigator.clipboard.writeText(document.getElementById(entryId).innerHTML); 
}

// might need to refresh somehow (possibly by turning this into a form)
function deleteItems() {
    let tmp = "";
    for (let i = 0; i < items.length; i++) {
        tmp = document.getElementById(i + "id");
        if (tmp.checked == true) {
            items.splice(i, 1);
        }
        console.log(items);
    }
}

// might need to refresh somehow (possibly by turning this into a form)
function addItem() { 
    let tmp = ["", "", "", ""];
    tmp[0] = document.getElementById("organization").value;
    tmp[1] = document.getElementById("email").value;
    tmp[2] = document.getElementById("username").value;
    tmp[3] = document.getElementById("password").value;
    if (tmp[0].length != 0 && tmp[1].length != 0 && tmp[2].length != 0 && tmp[3].length != 0) {
        items.push(tmp);
    }
    console.log (items);
}

// need to wait for html to render
window.onload = function() {
    document.getElementById("entries").innerHTML = `${generateItems()}`;

};

