import { group, check } from "k6";
import http from "k6/http";

export let options = {
    vus: 5,
    stages: [
        {   
            duration: "4m", target: 1000
            }
        ]
};
var url='34.72.210.159';

export default function () {
    group("front page", function () {
        check(http.get("http://"+url+"/en/home", { tags: { 'kind': 'html' }, }),
            { "status is 200": (res) => res.status === 200, }
        );
    });
}